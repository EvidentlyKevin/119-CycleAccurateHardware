"""
Implements a square systolic array for matrix-vector or matrix-matrix multiplication.

Raises:
    ValueError: If the dimensions of the weight matrix do not match the array size.
    ValueError: If the length of the input activation vector does not match the array size.

Returns:
    int: Total estimated cycles, including raw systolic compute cycles and modeled overhead.
    int: Total MAC (Multiply-Accumulate) operations performed.
"""

#-----------------------------------------------------------------------------------------------------------------
# Imports & Globals
#-----------------------------------------------------------------------------------------------------------------

mac_global_counter = 0  # Global MAC operation counter used for performance estimation

from mac_unit import MACUnit  # Import MAC processing unit class

#-----------------------------------------------------------------------------------------------------------------
# Classes
#-----------------------------------------------------------------------------------------------------------------

class SystolicArray:
    """
    Represents a 2D systolic array of MAC units for accelerating matrix operations.
    """

    def __init__(self, size):
        """
        Initialize the systolic array of given size (square matrix).

        Args:
            size (int): The height/width of the square systolic array.
        """
        self.SIZE = size
        self.array = [[MACUnit(i, j) for j in range(size)] for i in range(size)]

        # Connect all MAC units vertically for downward data flow
        for i in range(1, size):
            for j in range(size):
                self.array[i][j].set_up_in(self.array[i - 1][j].get_down_out())

    def set_weights(self, weights):
        """
        Load a 2D weight matrix into the array. Each MAC unit stores one weight.

        Args:
            weights (List[List[int]]): 2D matrix of weights (must match array size).

        Raises:
            ValueError: If weight matrix dimensions don't match the array size.
        """
        if len(weights) != self.SIZE or len(weights[0]) != self.SIZE:
            raise ValueError("Weights must match array dimensions.")
        for i in range(self.SIZE):
            for j in range(self.SIZE):
                self.array[i][j].set_weight(weights[i][j])

    def inject_activations(self, activations):
        """
        Inject a 1D input activation vector into the top row MAC units.

        Args:
            activations (List[int]): Input vector (must match width of array).

        Raises:
            ValueError: If input length doesn't match array size.
        """
        if len(activations) != self.SIZE:
            raise ValueError("Activation vector must match number of columns.")
        for j in range(self.SIZE):
            self.array[0][j].set_input_activation(activations[j])

    def cycle(self):
        """
        Simulates one clock cycle across all MAC units in the array.
        """
        for i in range(self.SIZE):
            for j in range(self.SIZE):
                self.array[i][j].cycle()

    def get_outputs(self):
        """
        Returns the output accumulators of all MAC units as a 2D matrix.

        Returns:
            List[List[int]]: Accumulator values from each MAC unit.
        """
        return [[self.array[i][j].read_accumulator() for j in range(self.SIZE)] for i in range(self.SIZE)]
    
    def get_full_output_matrix(self):
        """
        Same as get_outputs(). Provided for clarity and semantic distinction.

        Returns:
            List[List[int]]: Full 2D matrix of accumulator results.
        """
        return [[self.array[i][j].read_accumulator() for j in range(self.SIZE)] for i in range(self.SIZE)]

    def get_matrix_vector_result(self):
        """
        Collapses 2D accumulators into a 1D result vector (e.g., matrix-vector multiplication).

        Returns:
            List[int]: Sum of each row of accumulators.
        """
        return [sum(self.array[i][j].read_accumulator() for j in range(self.SIZE)) for i in range(self.SIZE)]

    def run_until_idle(self, max_cycles=10000000):
        """
        Simulates systolic array operation until all input queues are empty or a max cycle count is reached.
        Also estimates latency overhead using simple modeling parameters.

        Args:
            max_cycles (int, optional): Safety cap on number of clock cycles. Defaults to 10,000,000.

        Returns:
            Tuple[int, int]: Total estimated latency in cycles and total MAC operations performed.
        """
        global mac_global_counter
        mac_global_counter = 0

        active_cycles = 0
        while active_cycles < max_cycles:
            self.cycle()
            active_cycles += 1

            # Check if all data queues are empty (array is idle)
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
        M = N = K = self.SIZE  # Assume square matrix dimensions

        # Tunable coefficients for performance modeling
        alpha = 0.5    # DMA cycles per transferred element
        beta = 0.001   # Setup cost per MAC
        gamma = 0.05   # Stall penalty (as fraction of ideal throughput)

        macs = M * K * N  # Total number of MAC operations

        dma_cycles = int(alpha * (M * K + K * N + M * N))  # inputs + weights + outputs
        setup_cycles = int(beta * macs)
        ideal_cycles = macs / 65536  # Based on TPU throughput
        stall_cycles = int(gamma * ideal_cycles)

        overhead_cycles = dma_cycles + setup_cycles + stall_cycles
        total_cycles = active_cycles + overhead_cycles

        # Debug printout
        print(f"\n--- Overhead Breakdown ---")
        print(f"Raw systolic cycles: {active_cycles}")
        print(f" + DMA cycles:       {dma_cycles}")
        print(f" + Setup cycles:     {setup_cycles}")
        print(f" + Stall penalty:    {stall_cycles}")
        print(f" = Total estimated:  {total_cycles}")

        return total_cycles, mac_global_counter
