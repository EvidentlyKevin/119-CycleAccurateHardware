from channel import Channel

class MACUnit:
    MAX_CLK_STATE = 2
    CHANNEL_CAPACITY = 128
    CHANNEL_CAPACITY_INPUT = 10

    def __init__(self, row, col):
        self.rowID = row
        self.colID = col
        self.accumulator = 0
        self.a = 0
        self.b = 0
        self.w = 0
        self.clk = 0

        # Output channels
        self.rightOut = Channel(self.CHANNEL_CAPACITY)
        self.downOut = Channel(self.CHANNEL_CAPACITY)

        # Input activation queue (only for first row)
        self.inputA = Channel(self.CHANNEL_CAPACITY_INPUT) if row == 0 else None

        # Neighboring input channels
        self.leftIn = None
        self.upIn = None

    # Set input channels
    def set_up_in(self, up_channel):
        self.upIn = up_channel

    def set_left_in(self, left_channel):
        self.leftIn = left_channel

    # Get output channels
    def get_right_out(self):
        return self.rightOut

    def get_down_out(self):
        return self.downOut

    # Set weight
    def set_weight(self, weight):
        self.w = weight

    # Set activation (for first row)
    def set_input_activation(self, activation):
        if self.rowID == 0:
            self.inputA.push(activation)

    # Perform one cycle
    def cycle(self):
        if self.clk > self.MAX_CLK_STATE:
            self.clk = 0

        if self.clk == 0:
            if not self.fetch_inputs():
                return
            self.clk += 1
        elif self.clk == 1:
            self.compute_mac()
            self.clk += 1
        elif self.clk == 2:
            self.send_outputs()
            self.clk = 0

    # Fetch inputs for MAC operation
    def fetch_inputs(self):
        """Fetch activation from up and partial sum from left"""
        success = True

        if self.rowID == 0:
            if self.inputA and not self.inputA.is_empty():
                self.a = self.inputA.pop()
        else:
            if self.upIn and not self.upIn.is_empty():
                self.a = self.upIn.pop()
            else:
                success = False  # Activation not ready

        if self.colID == 0:
            self.b = 0
        else:
            if self.leftIn and not self.leftIn.is_empty():
                self.b = self.leftIn.pop()
            else:
                success = False  # Partial sum not ready

        print(f"MAC[{self.rowID}][{self.colID}] fetchInputs: a={self.a}, b={self.b}")
        return success

    # Compute MAC operation
    def compute_mac(self):
        self.accumulator = (self.a * self.w) + self.b
        print(f"MAC[{self.rowID}][{self.colID}] computeMAC: a={self.a}, w={self.w}, b={self.b}, accumulator={self.accumulator}")

    # Send outputs to right and down
    def send_outputs(self):
        if not self.rightOut.is_full():
            self.rightOut.push(self.accumulator)
            print(f"MAC[{self.rowID}][{self.colID}] sent accumulator {self.accumulator} to rightOut")

        if not self.downOut.is_full():
            self.downOut.push(self.a)
            print(f"MAC[{self.rowID}][{self.colID}] sent activation {self.a} to downOut")

    # Read accumulator value
    def read_accumulator(self):
        return self.accumulator
