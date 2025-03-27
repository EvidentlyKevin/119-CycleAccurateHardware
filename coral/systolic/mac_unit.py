from channel import Channel

class MACUnit:
    CHANNEL_CAPACITY = 128

    def __init__(self, row, col):
        self.rowID = row
        self.colID = col
        self.accumulator = 0
        self.w = 0
        self.a = 0

        # Output channel (to PE below)
        self.downOut = Channel(self.CHANNEL_CAPACITY)

        # Input channel from PE above
        self.upIn = None

        # Input queue only for top row
        self.inputA = Channel(self.CHANNEL_CAPACITY) if row == 0 else None

    def set_up_in(self, up_channel):
        self.upIn = up_channel

    def get_down_out(self):
        return self.downOut

    def set_weight(self, weight):
        self.w = weight

    def set_input_activation(self, activation):
        if self.rowID == 0:
            self.inputA.push(activation)

    def cycle(self):
        # Fetch input from above or input queue
        if self.rowID == 0:
            if not self.inputA.is_empty():
                self.a = self.inputA.pop()
            else:
                return  # No input to compute
        else:
            if self.upIn and not self.upIn.is_empty():
                self.a = self.upIn.pop()
            else:
                return  # No input to compute

        # MAC: Accumulate (weight * activation)
        self.accumulator += self.a * self.w
        print(f"MAC[{self.rowID}][{self.colID}] MAC: a={self.a}, w={self.w} => acc={self.accumulator}")

        # Pass activation downward
        if not self.downOut.is_full():
            self.downOut.push(self.a)

    def read_accumulator(self):
        return self.accumulator
