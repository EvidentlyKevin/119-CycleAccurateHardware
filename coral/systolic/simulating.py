"""
This module provides a configurable simulation interface for running systolic array
experiments with different matrix sizes, data types, weights, and activation inputs.
"""

#-----------------------------------------------------------------------------------------------------------------
# Imports
#-----------------------------------------------------------------------------------------------------------------

import numpy as np
from systolic_array import SystolicArray

#-----------------------------------------------------------------------------------------------------------------
# Functions
#-----------------------------------------------------------------------------------------------------------------

def generate_default_weights(size, dtype=int):
    """
    Generates a default weight matrix filled with ones.

    Args:
        size (int): Size of the square matrix.
        dtype (type): Data type for elements.

    Returns:
        List[List[dtype]]: Size x Size matrix of ones.
    """
    return np.ones((size, size), dtype=dtype).tolist()


def generate_default_inputs(size, dtype=int):
    """
    Generates a single input vector as a diagonal injection, then flushes the pipeline.

    Args:
        size (int): Systolic array width (assumes square).
        dtype (type): Data type (int, float16, etc.)

    Returns:
        List[List[dtype]]: Diagonal injection + zero padding.
    """
    vector = [dtype(i + 1) for i in range(size)]  # input vector: [1, 2, 3, ..., N]
    inputs = []

    for i in range(size):
        row = [0] * size
        row[i] = vector[i]  # inject one element diagonally
        inputs.append(row)

    for _ in range(size):  # flush the pipeline
        inputs.append([0] * size)

    return inputs


def simulate_array(
    size=4,
    dtype=int,
    weights=None,
    inputs=None,
    use_random_weights=False,
    use_random_inputs=False,
    weight_range=(0, 10),
    input_range=(0, 10)
):
    """
    Runs a configurable systolic array simulation.

    Args:
        size (int): Size of the square array (NxN).
        dtype (type): Data type for matrix entries (e.g. int, float16).
        weights (List[List]): Optional custom weights matrix.
        inputs (List[List]): Optional custom list of activation vectors.
        use_random_weights (bool): Whether to generate random weights.
        use_random_inputs (bool): Whether to generate random inputs.
        weight_range (tuple): Range for random weights (inclusive).
        input_range (tuple): Range for random inputs (inclusive).
    """
    sa = SystolicArray(size)

    # Configure weights
    if weights is not None:
        final_weights = weights
    elif use_random_weights:
        final_weights = np.random.randint(*weight_range, size=(size, size)).astype(dtype).tolist()
    else:
        final_weights = generate_default_weights(size, dtype)

    # print(final_weights)
    
    # Configure inputs
    if inputs is not None:
        final_inputs = inputs
    elif use_random_inputs:
        diag = np.random.randint(*input_range, size=size).astype(dtype)
        final_inputs = []
        for i in range(size):
            row = [0] * size
            row[i] = diag[i]
            final_inputs.append(row)
        for _ in range(size):
            final_inputs.append([0] * size)
    else:
        final_inputs = generate_default_inputs(size, dtype)

    # print(final_inputs)

    sa.set_weights(final_weights)
    for row in final_inputs:
        sa.inject_activations(row)

    # Run simulation
    cycles, macs = sa.run_until_idle()

    print(f"\n--- Simulation Results ---")
    print(f"Total Cycles: {cycles}")
    print(f"Total MACs: {macs}")
    # print(f"Output Vector (Row sums):")
    # for value in sa.get_matrix_vector_result():
    #     print(value)
    
def estimate_matrix_vector_cycles(size):
    """
    Estimates the ideal number of cycles for a size x size matrix multiply
    using a weight-stationary systolic array.

    Args:
        size (int): Matrix dimension (assumes square matrices).

    Returns:
        int: Ideal cycle count based on dataflow latency.
    """
    M = N = K = size
    return M + N + K - 2

def estimate_matrix_matrix_cycles(size):
    """
    Estimate ideal cycles for matrix-matrix multiplication (size x size).

    Returns:
        int: Total ideal cycles to complete all outputs.
    """
    M = N = K = size
    return K + 2 * M + N - 3

#-----------------------------------------------------------------------------------------------------------------
# Main
#-----------------------------------------------------------------------------------------------------------------

# Example usage
if __name__ == "__main__":
    # Default simulation (int type, weights=1, inputs=row values)
    # simulate_array()

    # Float16 with random weights/inputs
    # simulate_array(size=4, dtype=np.float16, use_random_weights=True, use_random_inputs=True)

    # Custom weights/inputs
    # simulate_array(size=3, weights=[[1,1,1],[1,1,1],[1,1,1]], inputs=[[1,1,1],[2,2,2],[3,4,3]])
    sizes = [4, 8, 16, 32, 64, 128, 256]
    for size in sizes:
        print(f"\n--- Running simulation for size {size} ---")
        matrix_vector = estimate_matrix_vector_cycles(size)
        print(f"Matrix-Vector Cycles (no overhead):        {matrix_vector}")
        matrix_matrix = estimate_matrix_matrix_cycles(size)
        print(f"Matrix-Matrix Cycles (no overhead):        {matrix_matrix}")
        simulate_array(size=size)
