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

test_systolic_array()
