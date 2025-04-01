from channel import Channel
import systolic_array

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
        if self.rowID == 0:
            if not self.inputA.is_empty():
                self.a = self.inputA.pop()
            else:
                return
        else:
            if self.upIn and not self.upIn.is_empty():
                self.a = self.upIn.pop()
            else:
                return

        # MAC operation
        if self.a != 0:
            self.accumulator += self.a * self.w
            systolic_array.mac_global_counter += 1

        if not self.downOut.is_full():
            self.downOut.push(self.a)

    def read_accumulator(self):
        return self.accumulator
