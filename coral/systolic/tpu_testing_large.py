import time
import numpy as np
import sys
from tflite_runtime.interpreter import Interpreter, load_delegate
from tqdm import tqdm  # Import tqdm for progress bars

def run_edge_tpu_inference(array_size, interpreter, input_data):
    # Get input/output details
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()

    # Set input tensor
    interpreter.set_tensor(input_details[0]['index'], input_data)

    # Run inference and time it
    start_time = time.perf_counter()
    interpreter.invoke()  # TPU inference
    end_time = time.perf_counter()

    # Get output tensor
    output_data = interpreter.get_tensor(output_details[0]['index'])

    # Read temperature
    with open("/sys/class/thermal/thermal_zone0/temp", "r") as f:
        temp = int(f.read().strip())

    # Use tqdm.write() to print while keeping progress bars intact
    sys.stdout.write(
        f"\rArray Size: {array_size} | "
        f"Inference Time: {(end_time - start_time) * 1e3:.3f} ms | "
        f"Temperature: {temp * 1e-3:.1f}°C  | "
    )
    sys.stdout.flush()

    return (end_time - start_time), temp

def warm_up_inference(interpreter, input_data, warm_up_iterations=10):
    """Run some warm-up iterations to avoid initialization overhead."""
    input_details = interpreter.get_input_details()  # Get input details inside the warm-up function
    for _ in range(warm_up_iterations):
        interpreter.set_tensor(input_details[0]['index'], input_data)  # Set input tensor
        interpreter.invoke()  # Run inference

def calculate_percentile(values, percentile):
    """Calculate the nth percentile of a list of values."""
    sorted_values = sorted(values)
    index = int(len(sorted_values) * percentile)
    return sorted_values[index]

if __name__ == "__main__":
    loop_num = 10000
    array_sizes = [4, 8, 16, 32, 64, 128, 256]
    total_times = {}
    cycle_counts = {}
    total_temps = {}
    all_run_times = {}
    all_cycle_counts = {}

    # Progress bar for array sizes
    for array_size in tqdm(array_sizes, desc="Testing Array Sizes"):
        total_times[f"{array_size}"] = 0
        cycle_counts[f"{array_size}"] = 0
        total_temps[f"{array_size}"] = 0
        all_run_times[f"{array_size}"] = []
        all_cycle_counts[f"{array_size}"] = []

        model_path = f"/home/mendel/119-CycleAccurateHardware/coral/systolic/matmul_{array_size}x{array_size}_int8_ones_edgetpu.tflite"
        input_data = np.random.randint(0, 10, size=(1, array_size)).astype(np.int8)

        interpreter = Interpreter(
            model_path=model_path,
            experimental_delegates=[load_delegate('libedgetpu.so.1')]
        )
        interpreter.allocate_tensors()

        # Warm-up phase to reduce delegate and interpreter overhead
        warm_up_inference(interpreter, input_data, warm_up_iterations=10)

        # Progress bar for loop iterations
        for i in tqdm(range(1, loop_num + 1), desc=f"Testing Size {array_size}", leave=False):
            run_time, temp = run_edge_tpu_inference(array_size, interpreter, input_data)

            # Store run times and cycle counts for percentile calculation
            all_run_times[f"{array_size}"].append(run_time)
            total_times[f"{array_size}"] += run_time
            total_temps[f"{array_size}"] += temp

            if temp < 85000:
                cycle_count = run_time * 500000000
            elif temp < 90000:
                cycle_count = run_time * 250000000
            elif temp < 95000:
                cycle_count = run_time * 125000000
            else:
                cycle_count = run_time * 62500000
            all_cycle_counts[f"{array_size}"].append(cycle_count)
            cycle_counts[f"{array_size}"] += cycle_count

    # Display final results with 1% high/low and average
    for array_size in array_sizes:
        average_time = total_times[f"{array_size}"] / loop_num
        avg_cycles = cycle_counts[f"{array_size}"] / loop_num
        avg_temp = total_temps[f"{array_size}"] / loop_num

        # Calculate 1% high and low for run times and cycle counts
        run_time_1_percent_low = calculate_percentile(all_run_times[f"{array_size}"], 0.01)
        run_time_1_percent_high = calculate_percentile(all_run_times[f"{array_size}"], 0.99)

        cycle_count_1_percent_low = calculate_percentile(all_cycle_counts[f"{array_size}"], 0.01)
        cycle_count_1_percent_high = calculate_percentile(all_cycle_counts[f"{array_size}"], 0.99)

        print(f"\n\n-------------------------------")
        print(f"       Array Size: {array_size}       ")
        print(f"Average Time: {average_time * 1e3:.3f} ms")
        print(f"Run Time 1% Low: {run_time_1_percent_low * 1e3:.3f} ms")
        print(f"Run Time 1% High: {run_time_1_percent_high * 1e3:.3f} ms")
        print(f"Average Temp: {avg_temp * 1e-3:.1f}°C")
        print(f"Average Cycle Count: {int(avg_cycles)}")
        print(f"Cycle Count 1% Low: {int(cycle_count_1_percent_low)}")
        print(f"Cycle Count 1% High: {int(cycle_count_1_percent_high)}")
