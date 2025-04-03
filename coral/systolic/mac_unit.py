"""
This module defines the MACUnit (Multiply-Accumulate Unit) class, which represents a
processing element in a systolic array. Each unit performs a basic MAC operation and 
communicates activations through channels with neighboring units vertically.

Classes:
    MACUnit: Represents a single MAC processing element in a systolic array row/column grid.
"""

#-----------------------------------------------------------------------------------------------------------------
# Imports
#-----------------------------------------------------------------------------------------------------------------

from channel import Channel           # For activation input/output buffering
import systolic_array                 # To increment global MAC operation counter

#-----------------------------------------------------------------------------------------------------------------
# Classes
#-----------------------------------------------------------------------------------------------------------------

class MACUnit:
    """
    A single Multiply-Accumulate (MAC) unit in a systolic array. Each unit stores a weight
    and receives activations from above or from an external input, performs multiplication,
    accumulates the result, and forwards the activation downward.

    Attributes:
        rowID (int): Row index of the MAC unit in the systolic array.
        colID (int): Column index of the MAC unit in the systolic array.
        accumulator (int): Stores the accumulated MAC result.
        w (int): The weight assigned to this MAC unit.
        a (int): The most recent activation value processed.
        downOut (Channel): Channel to send activations to the unit below.
        upIn (Channel or None): Channel to receive activations from the unit above.
        inputA (Channel or None): Only for top row units; stores activations to inject.
    """

    CHANNEL_CAPACITY = 512  # Max channel size for input/output buffering

    def __init__(self, row, col):
        """
        Initializes a MAC unit at a specified row and column in the systolic array.

        Args:
            row (int): Row index of this unit.
            col (int): Column index of this unit.
        """
        self.rowID = row
        self.colID = col
        self.accumulator = 0
        self.w = 0  # Initial weight
        self.a = 0  # Current activation

        # Channel to send activations down to the next unit in the column
        self.downOut = Channel(self.CHANNEL_CAPACITY)

        # Channel to receive activations from the unit above (set externally)
        self.upIn = None

        # Top row units receive inputs from an external source
        self.inputA = Channel(self.CHANNEL_CAPACITY) if row == 0 else None

    def set_up_in(self, up_channel):
        """
        Sets the input channel from the unit directly above.

        Args:
            up_channel (Channel): Channel providing activations from above.
        """
        self.upIn = up_channel

    def get_down_out(self):
        """
        Gets the channel used to pass activations to the unit below.

        Returns:
            Channel: The downward output channel.
        """
        return self.downOut

    def set_weight(self, weight):
        """
        Assigns a weight to this MAC unit.

        Args:
            weight (int): The weight to use in MAC operations.
        """
        self.w = weight

    def set_input_activation(self, activation):
        """
        Queues an activation value into the top-row MAC unit's input buffer.

        Args:
            activation (int): Activation value to enqueue (only valid on top row).
        """
        if self.rowID == 0:
            self.inputA.push(activation)

    def cycle(self):
        """
        Executes one clock cycle of computation:
            - Pulls an activation from input.
            - Performs the MAC operation: accumulator += a * w.
            - Pushes activation to the output channel (downward).
            - Increments the global MAC counter if a valid computation occurred.
        """
        # Input logic depends on whether this is a top-row MAC or not
        if self.rowID == 0:
            if not self.inputA.is_empty():
                self.a = self.inputA.pop()
            else:
                return  # No input available this cycle
        else:
            if self.upIn and not self.upIn.is_empty():
                self.a = self.upIn.pop()
            else:
                return  # No input available this cycle

        # Perform MAC if activation is non-zero
        if self.a != 0:
            self.accumulator += self.a * self.w
            systolic_array.mac_global_counter += 1  # Update global count

        # Propagate activation downward if possible
        if not self.downOut.is_full():
            self.downOut.push(self.a)

    def read_accumulator(self):
        """
        Returns the current accumulated MAC value in this unit.

        Returns:
            int: Accumulated result.
        """
        return self.accumulator
