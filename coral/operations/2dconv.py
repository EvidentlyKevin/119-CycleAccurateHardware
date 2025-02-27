def conv2d_python(image, kernel, padding=0, stride=1):
    """
    Perform a 2D convolution using pure Python (no NumPy).

    Parameters:
        image (list of lists): Input 2D matrix (image).
        kernel (list of lists): Kernel/filter for convolution.
        padding (int): Number of pixels to pad around the image.
        stride (int): Step size for sliding the kernel.

    Returns:
        list of lists: The convolved output matrix.
    """
    # Flip the kernel
    kernel = [row[::-1] for row in kernel[::-1]]  # Flipping both rows and columns

    # Get dimensions
    i_h, i_w = len(image), len(image[0])
    k_h, k_w = len(kernel), len(kernel[0])

    # Apply zero padding
    padded_image = [[0] * (i_w + 2 * padding) for _ in range(padding)]
    for row in image:
        padded_image.append([0] * padding + row + [0] * padding)
    padded_image += [[0] * (i_w + 2 * padding) for _ in range(padding)]

    # Compute output dimensions
    o_h = (i_h + 2 * padding - k_h) // stride + 1
    o_w = (i_w + 2 * padding - k_w) // stride + 1

    # Initialize output matrix
    output = [[0] * o_w for _ in range(o_h)]

    # Perform convolution
    for y in range(o_h):
        for x in range(o_w):
            conv_sum = 0
            for ky in range(k_h):
                for kx in range(k_w):
                    conv_sum += padded_image[y * stride + ky][x * stride + kx] * kernel[ky][kx]
            output[y][x] = conv_sum

    return output

# Example usage
image = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
kernel = [[1, 0], [0, -1]]
result = conv2d_python(image, kernel, padding=1)
for row in result:
    print(row)
