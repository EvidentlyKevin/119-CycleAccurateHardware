import time
import numpy as np
from tflite_runtime.interpreter import Interpreter, load_delegate

def run_edge_tpu_inference():
    model_path = "/home/mendel/119-CycleAccurateHardware/coral/systolic/matmul_32_model_edgetpu.tflite"
    input_data = np.random.randint(0, 10, size=(1, 32)).astype(np.float32)  # shape: (1, 3)

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

    print(f"Input: {input_data.flatten().tolist()}")
    print(f"Output: {output_data.flatten().tolist()}")
    print(f"Inference Time: {(end_time - start_time) * 1e3:.3f} ms")

if __name__ == "__main__":
    run_edge_tpu_inference()
