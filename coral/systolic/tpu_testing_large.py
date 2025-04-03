import time
import numpy as np
import os
from tflite_runtime.interpreter import Interpreter, load_delegate

def run_edge_tpu_inference(array_size):
    model_path = f"/home/mendel/119-CycleAccurateHardware/coral/systolic/matmul_{array_size}_model_edgetpu.tflite"
    input_data = np.random.randint(0, 10, size=(1, array_size)).astype(np.float32)  # shape: (1, 3)

    # Load Edge TPU-compiled model with TPU delegate
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

    # Run inference and time it
    start_time = time.perf_counter()
    interpreter.invoke()
    end_time = time.perf_counter()

    # Get output tensor
    output_data = interpreter.get_tensor(output_details[0]['index'])

    #print(f"Input: {input_data.flatten().tolist()}")
    #print(f"Output: {output_data.flatten().tolist()}")
    print(f"Inference Time: {(end_time - start_time) * 1e3:.3f} ms")
    print(f"Temperature: {os.system('cat /sys/class/thermal/thermal_zone0/temp')}")

    return (end_time-start_time) * 1e3

if __name__ == "__main__":
    loop_num = 50
    total = 0
    min_time = 0
    max_time = 0
    array_sizes = [4, 16, 32, 64, 128, 256]
    total_times = {}
    for array_size in array_sizes:
        total_times[f"{array_size}"] = 0
        for i in range(1, loop_num+1):
            #time.sleep(0.01) 
            run_time = run_edge_tpu_inference(array_size)
            if(min_time == 0 or min_time > run_time):
                min_time = run_time
            if(max_time == 0 or max_time < run_time):
                max_time = run_time
            total_times[f"{array_size}"] += run_time
    for array_size in array_sizes:
        average=total_times[f"{array_sizes}"]/loop_num
        print(f"\n\n-------------------------------")
        print(f"       Array Size: {array_size}       ")
        print(f"Min Run Time: {min_time:.3f} ms")
        print(f"Max Run Time: {max_time:.3f} ms")
        print(f"Average Time: {average:.3f} ms")


