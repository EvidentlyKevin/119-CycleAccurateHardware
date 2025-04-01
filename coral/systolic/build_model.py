import tensorflow as tf
import numpy as np

def build_matmul_model():
    # Define input vector x (placeholder shape)
    input_shape = (1, 3)  # batch size 1, vector length 3
    x_input = tf.keras.Input(shape=(3,), batch_size=1, name='x')

    # Define constant weight matrix W
    weights = np.array([
        [1, 4, 7],
        [2, 5, 8],
        [3, 6, 9]
    ], dtype=np.float32)

    # Dense layer with no bias
    y_output = tf.keras.layers.Dense(
        units=3,
        use_bias=False,
        kernel_initializer=tf.constant_initializer(weights)
    )(x_input)

    model = tf.keras.Model(inputs=x_input, outputs=y_output)
    model.summary()

    return model
    
def build_large_matmul_model():
    # Input vector x with shape (1, 256)
    x_input = tf.keras.Input(shape=(256,), batch_size=1, name='x')

    # Random fixed weight matrix W (256x256)
    weights = np.random.randint(0, 10, size=(256, 256)).astype(np.float32)

    # Dense layer to simulate W @ x
    y_output = tf.keras.layers.Dense(
        units=256,
        use_bias=False,
        kernel_initializer=tf.constant_initializer(weights)
    )(x_input)

    model = tf.keras.Model(inputs=x_input, outputs=y_output)
    model.summary()
    return model

def convert_to_tflite(model, filename='matmul_256_model.tflite'):
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]  # Allow quantization
    tflite_model = converter.convert()

    with open(filename, 'wb') as f:
        f.write(tflite_model)
    print(f"TFLite model written to {filename}")
    
def build_matmul_model_64(size=64, filename='matmul_64_model.tflite'):
    # Input: vector of shape (1, size)
    x_input = tf.keras.Input(shape=(size,), batch_size=1, name='x')

    # Random weight matrix of shape (size, size)
    weights = np.random.randint(0, 10, size=(size, size)).astype(np.float32)

    # Dense layer with no bias
    y_output = tf.keras.layers.Dense(
        units=size,
        use_bias=False,
        kernel_initializer=tf.constant_initializer(weights)
    )(x_input)

    model = tf.keras.Model(inputs=x_input, outputs=y_output)
    model.summary()

    # Convert to TFLite
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    tflite_model = converter.convert()

    with open(filename, 'wb') as f:
        f.write(tflite_model)
    print(f"TFLite model written to: {filename}")
    
def build_matmul_model_128(size=128, filename='matmul_128_model.tflite'):
    x_input = tf.keras.Input(shape=(size,), batch_size=1, name='x')

    weights = np.random.randint(0, 10, size=(size, size)).astype(np.float32)

    y_output = tf.keras.layers.Dense(
        units=size,
        use_bias=False,
        kernel_initializer=tf.constant_initializer(weights)
    )(x_input)

    model = tf.keras.Model(inputs=x_input, outputs=y_output)
    model.summary()

    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    tflite_model = converter.convert()

    with open(filename, 'wb') as f:
        f.write(tflite_model)
    print(f"TFLite model written to: {filename}")
    
def build_matmul_model_32(size=32, filename='matmul_32_model.tflite'):
    # Input: vector of shape (1, size)
    x_input = tf.keras.Input(shape=(size,), batch_size=1, name='x')

    # Random weight matrix of shape (size, size)
    weights = np.random.randint(0, 10, size=(size, size)).astype(np.float32)

    # Dense layer with no bias
    y_output = tf.keras.layers.Dense(
        units=size,
        use_bias=False,
        kernel_initializer=tf.constant_initializer(weights)
    )(x_input)

    model = tf.keras.Model(inputs=x_input, outputs=y_output)
    model.summary()

    # Convert to TFLite
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    tflite_model = converter.convert()

    with open(filename, 'wb') as f:
        f.write(tflite_model)
    print(f"TFLite model written to: {filename}")
    
def build_matmul_model_16(size=16, filename='matmul_16_model.tflite'):
    # Input: vector of shape (1, size)
    x_input = tf.keras.Input(shape=(size,), batch_size=1, name='x')

    # Random weight matrix of shape (size, size)
    weights = np.random.randint(0, 10, size=(size, size)).astype(np.float32)

    # Dense layer with no bias
    y_output = tf.keras.layers.Dense(
        units=size,
        use_bias=False,
        kernel_initializer=tf.constant_initializer(weights)
    )(x_input)

    model = tf.keras.Model(inputs=x_input, outputs=y_output)
    model.summary()

    # Convert to TFLite
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    tflite_model = converter.convert()

    with open(filename, 'wb') as f:
        f.write(tflite_model)
    print(f"TFLite model written to: {filename}")
    
def build_matmul_model_4(size=4, filename='matmul_4_model.tflite'):
    # Input: vector of shape (1, size)
    x_input = tf.keras.Input(shape=(size,), batch_size=1, name='x')

    # Random weight matrix of shape (size, size)
    weights = np.random.randint(0, 10, size=(size, size)).astype(np.float32)

    # Dense layer with no bias
    y_output = tf.keras.layers.Dense(
        units=size,
        use_bias=False,
        kernel_initializer=tf.constant_initializer(weights)
    )(x_input)

    model = tf.keras.Model(inputs=x_input, outputs=y_output)
    model.summary()

    # Convert to TFLite
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    tflite_model = converter.convert()

    with open(filename, 'wb') as f:
        f.write(tflite_model)
    print(f"TFLite model written to: {filename}")

if __name__ == '__main__':
    # model = build_matmul_model()
    model = build_large_matmul_model()
    # model = build_matmul_model_64()
    # model = build_matmul_model_128()
    # model = build_matmul_model_4()
    # model = build_matmul_model_32()
    # model = build_matmul_model_16()
    convert_to_tflite(model)