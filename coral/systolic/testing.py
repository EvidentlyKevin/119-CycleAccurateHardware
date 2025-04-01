import numpy as np
from systolic_array import SystolicArray


def test_systolic_array():
    size = 2
    systolic_array = SystolicArray(size)

    weights = [[2, 3], [4, 5]]
    activations = [1, 2]

    systolic_array.set_weights(weights)
    systolic_array.set_input_activations(activations)

    for cycle in range(6):
        print(f"\n--- Cycle {cycle} ---")
        systolic_array.cycle()

    outputs = systolic_array.get_outputs()
    print("\nFinal outputs:", outputs)

# test_systolic_array()

def test_weight_stationary_id():
    size = 3
    sa = SystolicArray(size)

    weights = [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ]

    inputs = [
        [1, 0, 0],
        [0, 1, 0],
        [0, 0, 1]  # Acts like identity input
    ]

    sa.set_weights(weights)

    for cycle, row in enumerate(inputs):
        print(f"\n--- Cycle {cycle} (Inject: {row}) ---")
        sa.inject_activations(row)
        sa.cycle()

    print("\nFinal Accumulators (Output Matrix):")
    for row in sa.get_outputs():
        print(row)

test_weight_stationary_id()

def test_weight_stationary():
    size = 3
    sa = SystolicArray(size)

    weights = [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ]

    inputs = [
        [1, 0, 0],  # x0 enters column 0
        [0, 2, 0],  # x1 enters column 1
        [0, 0, 3],  # x2 enters column 2
    ]


    sa.set_weights(weights)

    for cycle, row in enumerate(inputs):
        print(f"\n--- Cycle {cycle} (Inject: {row}) ---")
        sa.inject_activations(row)
        sa.cycle()

    print("\nFinal Accumulators (Output Matrix):")
    for row in sa.get_outputs():
        print(row)

test_weight_stationary()

def test_mac_cycle_count():
    size = 3
    sa = SystolicArray(size)

    weights = [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ]
    # weights = [
    #     [1, 4, 7],
    #     [2, 5, 8],
    #     [3, 6, 9]
    # ]

    inputs = [
        [1, 0, 0],  # x0
        [0, 2, 0],  # x1
        [0, 0, 3],  # x2
        [0, 0, 0],
        [0, 0, 0],
    ]

    sa.set_weights(weights)
    for row in inputs:
        sa.inject_activations(row)

    cycles, macs = sa.run_until_idle()

    print(f"\nTotal Cycles: {cycles}")
    print(f"Total MACs performed: {macs}")
    print("\nFinal Output (Summed Rows):")
    for row in sa.get_matrix_vector_result():  # sum of each row
        print(row)

test_mac_cycle_count()

def test_systolic_array_256x256():
    size = 256
    sa = SystolicArray(size)

    # Generate random 8-bit weights and input vector
    weights = np.random.randint(0, 10, size=(size, size)).tolist()
    vector = np.random.randint(0, 10, size=(size,)).tolist()

    # Format the vector as a series of "diagonal" injection rows
    inputs = []
    for i in range(size):
        row = [0] * size
        row[i] = vector[i]
        inputs.append(row)
    
    # Add padding cycles to flush through the array
    for _ in range(size):
        inputs.append([0] * size)

    sa.set_weights(weights)
    for row in inputs:
        sa.inject_activations(row)

    # Run and count cycles and MACs
    cycles, macs = sa.run_until_idle()

    print(f"\nTotal Cycles: {cycles}")
    print(f"Total MACs performed: {macs}")

    output_vector = sa.get_matrix_vector_result()
    print("\nOutput vector (first 10 values):")
    print(output_vector[:10])  # Print first 10 for brevity

test_systolic_array_256x256()