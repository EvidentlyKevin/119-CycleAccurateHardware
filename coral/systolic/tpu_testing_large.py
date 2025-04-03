import time
import numpy as np
import sys
from tflite_runtime.interpreter import Interpreter, load_delegate
from tqdm import tqdm  # Import tqdm for progress bars

def run_edge_tpu_inference(array_size):
    model_path = f"/home/mendel/119-CycleAccurateHardware/coral/systolic/matmul_{array_size}x{array_size}_int8_ones_edgetpu.tflite"
    input_data = np.random.randint(0, 10, size=(1, array_size)).astype(np.int8)

    interpreter = Interpreter(
        model_path=model_path,
        experimental_delegates=[load_delegate('libedgetpu.so.1')]
    )
    interpreter.allocate_tensors()

    # Get input/output details
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()

    # Set input tensor
    interpreter.set_tensor(input_details[0]['index'], input_data)

    for detail in interpreter.get_tensor_details():
        print(detail['name'], detail['dtype'], detail['shape'])


    # Run inference and time it
    start_time = time.perf_counter()
    interpreter.invoke()
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

if __name__ == "__main__":
    loop_num = 1000
    array_sizes = [4, 8, 16, 32, 64, 128, 256]
    min_time = {}
    max_time = {}
    total_times = {}
    cycle_counts = {}
    total_temps = {}

    # Progress bar for array sizes
    for array_size in tqdm(array_sizes, desc="Testing Array Sizes"):
        total_times[f"{array_size}"] = 0
        min_time[f"{array_size}"] = 0
        max_time[f"{array_size}"] = 0
        cycle_counts[f"{array_size}"] = 0
        total_temps[f"{array_size}"] = 0

        # Progress bar for loop iterations
        for i in tqdm(range(1, loop_num+1), desc=f"Testing Size {array_size}", leave=False):
            run_time, temp = run_edge_tpu_inference(array_size)

            if min_time[f"{array_size}"] == 0 or min_time[f"{array_size}"] > run_time:
                min_time[f"{array_size}"] = run_time
            if max_time[f"{array_size}"] == 0 or max_time[f"{array_size}"] < run_time:
                max_time[f"{array_size}"] = run_time

            total_times[f"{array_size}"] += run_time
            total_temps[f"{array_size}"] += temp

            if temp < 85000:
                cycle_counts[f"{array_size}"] += run_time * 500000000
            elif temp < 90000:
                cycle_counts[f"{array_size}"] += run_time * 250000000
            elif temp < 95000:
                cycle_counts[f"{array_size}"] += run_time * 125000000
            else:
                cycle_counts[f"{array_size}"] += run_time * 62500000

    # Display final results
    for array_size in array_sizes:
        average = total_times[f"{array_size}"] / loop_num
        avg_cycles = cycle_counts[f"{array_size}"] / loop_num
        avg_temp = total_temps[f"{array_size}"] / loop_num

        print(f"\n\n-------------------------------")
        print(f"       Array Size: {array_size}       ")
        print(f"Min Run Time: {min_time[f'{array_size}']*1e3:.3f} ms")
        print(f"Max Run Time: {max_time[f'{array_size}']*1e3:.3f} ms")
        print(f"Average Time: {average*1e3:.3f} ms")
        print(f"Average Temp: {avg_temp * 1e-3}°C")
        print(f"Average Cycle Count: {avg_cycles}")
