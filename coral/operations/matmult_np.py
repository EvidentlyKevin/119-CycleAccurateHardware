import numpy as np

def matrix_multiply_numpy(A, B):
    return np.dot(A, B)

# Example usage
A = np.array([[1, 2], [3, 4]])
B = np.array([[5, 6], [7, 8]])
result = matrix_multiply_numpy(A, B)
print(result)
