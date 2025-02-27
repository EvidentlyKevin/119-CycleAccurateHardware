import numpy as np

def conv2d_numpy(image, kernel, padding=0, stride=1):
    """
    Perform a 2D convolution using NumPy.

    Parameters:
        image (ndarray): Input 2D matrix (image).
        kernel (ndarray): Kernel/filter for convolution.
        padding (int): Number of pixels to pad around the image.
        stride (int): Step size for sliding the kernel.

    Returns:
        ndarray: The convolved output matrix.
    """
    # Flip the kernel for convolution
    kernel = np.flipud(np.fliplr(kernel))

    # Get dimensions
    i_h, i_w = image.shape
    k_h, k_w = kernel.shape

    # Pad the image
    padded_image = np.pad(image, padding, mode='constant', constant_values=0)

    # Compute output dimensions
    o_h = (i_h + 2 * padding - k_h) // stride + 1
    o_w = (i_w + 2 * padding - k_w) // stride + 1

    # Initialize output matrix
    output = np.zeros((o_h, o_w))

    # Perform convolution
    for y in range(o_h):
        for x in range(o_w):
            region = padded_image[y * stride:y * stride + k_h, x * stride:x * stride + k_w]
            output[y, x] = np.sum(region * kernel)

    return output

# Example usage
image = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
kernel = np.array([[1, 0], [0, -1]])
result = conv2d_numpy(image, kernel, padding=1)
print(result)
