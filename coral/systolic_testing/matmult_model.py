# Guide: https://chatgpt.com/c/67c0b233-9ff8-8000-9be1-a4c3f3802619

import tensorflow as tf
import numpy as np

# Define a simple model with matrix multiplication
class MatrixMultiplyModel(tf.Module):
    def __init__(self):
        super().__init__()
        self.W = tf.Variable(np.random.rand(4, 4).astype(np.float32), name="W")

    @tf.function(input_signature=[tf.TensorSpec(shape=[4, 4], dtype=tf.float32)])
    def multiply(self, x):
        return tf.matmul(x, self.W)

# Create model instance
model = MatrixMultiplyModel()
# Convert to TensorFlow Lite
converter = tf.lite.TFLiteConverter.from_concrete_functions([model.multiply.get_concrete_function()])
tflite_model = converter.convert()

# Save model to a file
with open("matrix_multiply.tflite", "wb") as f:
    f.write(tflite_model)

print("TFLite model saved!")