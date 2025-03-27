from mac_unit import MACUnit

class SystolicArray:
    def __init__(self, size):
        self.SIZE = size
        self.array = [[MACUnit(i, j) for j in range(size)] for i in range(size)]

        # Connect PEs vertically (for downward activation flow)
        for i in range(1, size):
            for j in range(size):
                self.array[i][j].set_up_in(self.array[i - 1][j].get_down_out())

    def set_weights(self, weights):
        if len(weights) != self.SIZE or len(weights[0]) != self.SIZE:
            raise ValueError("Weights must match array dimensions.")
        for i in range(self.SIZE):
            for j in range(self.SIZE):
                self.array[i][j].set_weight(weights[i][j])

    def inject_activations(self, activations):
        if len(activations) != self.SIZE:
            raise ValueError("Activation vector must match number of columns.")
        for j in range(self.SIZE):
            self.array[0][j].set_input_activation(activations[j])

    def cycle(self):
        for i in range(self.SIZE):
            for j in range(self.SIZE):
                self.array[i][j].cycle()

    def get_outputs(self):
        return [[self.array[i][j].read_accumulator() for j in range(self.SIZE)] for i in range(self.SIZE)]
