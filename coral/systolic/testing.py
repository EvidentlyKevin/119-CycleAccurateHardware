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