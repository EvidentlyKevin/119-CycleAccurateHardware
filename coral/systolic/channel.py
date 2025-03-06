from collections import deque

class Channel:
    DEFAULT_CAPACITY = 128

    def __init__(self, capacity=DEFAULT_CAPACITY):
        """Initialize a channel with a given capacity."""
        self.capacity = capacity
        self.buffer = deque()

    def push(self, element):
        """Push an element into the channel. Returns False if full."""
        if len(self.buffer) >= self.capacity:
            return False
        self.buffer.append(element)
        return True

    def pop(self):
        """Pop an element from the channel. Returns None if empty."""
        if self.buffer:
            return self.buffer.popleft()
        return None

    def is_full(self):
        """Check if the channel is full."""
        return len(self.buffer) >= self.capacity

    def is_empty(self):
        """Check if the channel is empty."""
        return len(self.buffer) == 0

    def size(self):
        """Return the current size of the channel."""
        return len(self.buffer)
