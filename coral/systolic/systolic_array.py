from mac_unit import MACUnit

class SystolicArray:
    DEFAULT_SIZE = 8

    def __init__(self, size=DEFAULT_SIZE):
        self.SIZE = size
        self.array = [[MACUnit(i, j) for j in range(size)] for i in range(size)]

        # Connect MAC units
        for i in range(size):
            for j in range(size):
                if j > 0:
                    self.array[i][j].set_left_in(self.array[i][j - 1].get_right_out())
                if i > 0:
                    self.array[i][j].set_up_in(self.array[i - 1][j].get_down_out())

    def set_weights(self, weights):
        if len(weights) != self.SIZE or len(weights[0]) != self.SIZE:
            raise ValueError("Weights matrix must match systolic array size.")
        for i in range(self.SIZE):
            for j in range(self.SIZE):
                self.array[i][j].set_weight(weights[i][j])

    def set_input_activations(self, activations):
        if len(activations) != self.SIZE:
            raise ValueError("Activation vector must match array width.")
        for j in range(self.SIZE):
            self.array[0][j].set_input_activation(activations[j])

    def cycle(self):
        for i in range(self.SIZE):
            for j in range(self.SIZE):
                self.array[i][j].cycle()

    def get_outputs(self):
        return [self.array[i][-1].read_accumulator() for i in range(self.SIZE)]
