import tflite_runtime.interpreter as tflite

# Load Edge TPU model
interpreter = tflite.Interpreter(model_path="matrix_multiply_edgetpu.tflite")
interpreter.allocate_tensors()

# Get input/output tensors
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

# Provide input data
input_data = np.random.rand(4, 4).astype(np.float32)
interpreter.set_tensor(input_details[0]['index'], input_data)

# Run inference
interpreter.invoke()

# Get the result
output_data = interpreter.get_tensor(output_details[0]['index'])
print("Matrix Multiplication Output:\n", output_data)