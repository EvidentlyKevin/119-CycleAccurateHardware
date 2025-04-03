"""
This module defines a FIFO communication channel used to transfer data between 
processing elements (PEs) in a systolic array. Each channel supports basic 
queue operations with optional capacity control to simulate hardware-level buffer limits.

Classes:
    Channel: Represents a fixed-capacity queue with push/pop methods and utility checks.
"""

#-----------------------------------------------------------------------------------------------------------------
# Imports
#-----------------------------------------------------------------------------------------------------------------

from collections import deque  # Used to implement a fast FIFO queue

#-----------------------------------------------------------------------------------------------------------------
# Classes
#-----------------------------------------------------------------------------------------------------------------

class Channel:
    """
    A fixed-size FIFO (First-In-First-Out) channel used for modeling 
    data communication between processing elements in systolic arrays.

    Attributes:
        DEFAULT_CAPACITY (int): Default maximum buffer size (128 elements).
        capacity (int): Maximum number of elements the channel can store.
        buffer (deque): Underlying double-ended queue storing elements.
    """
    DEFAULT_CAPACITY = 128

    def __init__(self, capacity=DEFAULT_CAPACITY):
        """
        Initializes a Channel instance with the specified capacity.

        Args:
            capacity (int, optional): Maximum number of items that can be stored. 
                                      Defaults to DEFAULT_CAPACITY.
        """
        self.capacity = capacity
        self.buffer = deque()

    def push(self, element):
        """
        Attempts to push an element into the channel.

        Args:
            element (any): The item to enqueue.

        Returns:
            bool: False if the channel is full, True if the item was added.
        """
        if len(self.buffer) >= self.capacity:
            return False  # Reject new elements if capacity is reached
        self.buffer.append(element)
        return True

    def pop(self):
        """
        Pops an element from the channel if not empty.

        Returns:
            any or None: The dequeued element, or None if the channel is empty.
        """
        if self.buffer:
            return self.buffer.popleft()
        return None

    def is_full(self):
        """
        Checks whether the channel has reached its maximum capacity.

        Returns:
            bool: True if full, False otherwise.
        """
        return len(self.buffer) >= self.capacity

    def is_empty(self):
        """
        Checks whether the channel is empty.

        Returns:
            bool: True if empty, False otherwise.
        """
        return len(self.buffer) == 0

    def size(self):
        """
        Gets the current number of items in the channel.

        Returns:
            int: The number of items in the buffer.
        """
        return len(self.buffer)
