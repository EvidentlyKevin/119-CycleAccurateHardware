"""
This module defines utilities for building and exporting fully quantized
matrix multiplication models compatible with the Edge TPU.

Functions:
    build_flexible_matmul_model(): Builds a configurable matmul model.
    convert_to_tflite_edge_tpu(): Converts the model with int8 quantization.
    build_and_convert_edge_tpu_model(): Full pipeline to create & save the TFLite model.
"""

#-----------------------------------------------------------------------------------------------------------------
# Imports
#-----------------------------------------------------------------------------------------------------------------

import tensorflow as tf
import numpy as np
import os

#-----------------------------------------------------------------------------------------------------------------
# Functions
#-----------------------------------------------------------------------------------------------------------------

def representative_data_gen(size):
    """
    Yields representative input samples for post-training quantization.

    Args:
        size (int): Size of the input tensor.
    """
    for _ in range(100):
        yield [np.random.randint(0, 10, size=(1, size)).astype(np.float32)]

def build_flexible_matmul_model(
    size,
    dtype=np.float32,
    weights=None,
    use_random_weights=False,
    weight_range=(0, 10)
):
    """
    Builds a dense matmul model with flexible weight options.

    Args:
        size (int): Width of the input/output vectors.
        dtype (type): Tensor data type (must start as float32 for quantization).
        weights (np.ndarray or None): Optional weight matrix.
        use_random_weights (bool): Whether to initialize weights randomly.
        weight_range (tuple): Range for random weights.

    Returns:
        tf.keras.Model: Keras model instance.
        str: Type of weight init used ("ones", "random", or "custom").
    """
    x_input = tf.keras.Input(shape=(size,), batch_size=1, name='x', dtype=dtype)

    if weights is not None:
        weight_matrix = np.array(weights, dtype=dtype)
        weight_type = "custom"
    elif use_random_weights:
        weight_matrix = np.random.randint(*weight_range, size=(size, size)).astype(dtype)
        weight_type = "random"
    else:
        weight_matrix = np.ones((size, size), dtype=dtype)
        weight_type = "ones"

    y_output = tf.keras.layers.Dense(
        units=size,
        use_bias=False,
        kernel_initializer=tf.constant_initializer(weight_matrix),
        dtype=dtype
    )(x_input)

    model = tf.keras.Model(inputs=x_input, outputs=y_output)
    model.summary()
    return model, weight_type


def convert_to_tflite_edge_tpu(model, size, weight_type, filename=None):
    """
    Converts and saves a TFLite int8 quantized model for Edge TPU.

    Args:
        model (tf.keras.Model): The model to convert.
        size (int): Input/output dimension.
        weight_type (str): "ones", "random", or "custom" for naming.
        filename (str or None): Optional custom filename.
    """
    converter = tf.lite.TFLiteConverter.from_keras_model(model)

    # Full integer quantization (Edge TPU-compatible)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
    converter.inference_input_type = tf.int8
    converter.inference_output_type = tf.int8
    converter.representative_dataset = lambda: representative_data_gen(size)

    tflite_model = converter.convert()

    # Save
    if filename is None:
        filename = f"matmul_{size}x{size}_int8_{weight_type}.tflite"

    os.makedirs(os.path.dirname(filename) or ".", exist_ok=True)

    with open(filename, "wb") as f:
        f.write(tflite_model)
    print(f"✅ Quantized TFLite model written to: {filename}")


def build_and_convert_edge_tpu_model(
    size,
    weights=None,
    use_random_weights=False,
    weight_range=(0, 10),
    filename=None
):
    """
    Builds and saves an Edge TPU-compatible TFLite model with post-training quantization.

    Args:
        size (int): Size of the square matmul model.
        weights (np.ndarray or None): Optional custom weights.
        use_random_weights (bool): Use random weights if True.
        weight_range (tuple): Range for random weights.
        filename (str or None): Optional manual filename.
    """
    model, weight_type = build_flexible_matmul_model(
        size=size,
        dtype=np.float32,  # Must start in float for PTQ
        weights=weights,
        use_random_weights=use_random_weights,
        weight_range=weight_range
    )

    convert_to_tflite_edge_tpu(
        model=model,
        size=size,
        weight_type=weight_type,
        filename=filename
    )


#-----------------------------------------------------------------------------------------------------------------
# Example Usage
#-----------------------------------------------------------------------------------------------------------------

if __name__ == "__main__":
    # # Default weights = 1s
    # build_and_convert_edge_tpu_model(size=8)

    # # Random weights
    # build_and_convert_edge_tpu_model(size=16, use_random_weights=True)

    # # Custom weights
    # custom = np.arange(9).reshape(3, 3)
    # build_and_convert_edge_tpu_model(size=3, weights=custom)

    # Example batch of model builds and exports for benchmarking/testing purposes
    sizes = [4, 8, 16, 32, 64, 128, 256]
    for size in sizes:
        build_and_convert_edge_tpu_model(size, filename=None)
