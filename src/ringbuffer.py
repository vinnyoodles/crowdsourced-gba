class RingBuffer:
    def __init__(self, size):
        self.index = 0
        self.size = size
        self._data = []

    def append(self, value):
        if len(self._data) == self.size:
            self._data[self.index] = value
        else:
            self._data.append(value)
        self.index = (self.index + 1) % self.size

    def get_k_recent(self, key):
        if key > 1:
            if len(self._data) == self.size:
                # starting index + k recent
                return self._data[(key - 1) % self.size]
            else:
                return self._data[key]
        else:
            return 'Invalid Lookup'
