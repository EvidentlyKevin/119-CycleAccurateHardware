mac_global_counter = 0

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
    
    def get_full_output_matrix(self):
        return [[self.array[i][j].read_accumulator() for j in range(self.SIZE)] for i in range(self.SIZE)]

    def get_matrix_vector_result(self):
        return [sum(self.array[i][j].read_accumulator() for j in range(self.SIZE)) for i in range(self.SIZE)]

    def run_until_idle(self, max_cycles=10000000):
        global mac_global_counter
        mac_global_counter = 0

        active_cycles = 0
        while active_cycles < max_cycles:
            self.cycle()
            active_cycles += 1

            # Check if all input queues are empty
            all_empty = True
            for i in range(self.SIZE):
                for j in range(self.SIZE):
                    unit = self.array[i][j]
                    if unit.rowID == 0 and not unit.inputA.is_empty():
                        all_empty = False
                    if unit.upIn and not unit.upIn.is_empty():
                        all_empty = False
            if all_empty:
                break

        # === Dynamic Overhead Estimation ===
        M = N = K = self.SIZE  # Square matrices

        # Tunable coefficients for modeling real-world latency overheads
        alpha = 0.5    # DMA cost per transferred element (cycles)
        beta = 0.001   # Setup cost per MAC (cycles)
        gamma = 0.05   # Stall penalty (fraction of ideal compute cycles)

        # Operation counts
        macs = M * K * N

        # Overhead computations
        dma_cycles = int(alpha * (M * K + K * N + M * N))  # inputs + weights + outputs
        setup_cycles = int(beta * macs)
        ideal_cycles = macs / 65536  # Ideal throughput on a 256x256 TPU
        stall_cycles = int(gamma * ideal_cycles)

        # Total estimate
        overhead_cycles = dma_cycles + setup_cycles + stall_cycles
        total_cycles = active_cycles + overhead_cycles

        print(f"\n--- Overhead Breakdown ---")
        print(f"Raw systolic cycles: {active_cycles}")
        print(f" + DMA cycles:       {dma_cycles}")
        print(f" + Setup cycles:     {setup_cycles}")
        print(f" + Stall penalty:    {stall_cycles}")
        print(f" = Total estimated:  {total_cycles}")

        return total_cycles, mac_global_counter
