class RingBuffer:
    def __init__(self, size):
        self.index = 0
        self.size = size
        self._data = []
        self.last_elem = 0
        self.first_elem = 0

    def append(self, value):
        if len(self._data) == self.size:
            self.first_elem = (self.first_elem + 1) % self.size
            self._data[self.index] = value
        else:
            self._data.append(value)
        self.last_elem = self.index
        self.index = (self.index + 1) % self.size


    def get_k_recent(self, key):
        k_recents = list()
        count = self.last_elem
        if key <= 0:
            raise LookupError("Invalid Lookup")
        else:
            while key != 0 and count != self.first_elem:
                k_recents.append(self._data[count])
                count -= 1
                if count < 0:
                    count = self.size - 1
                key -= 1
            k_recents.append(self._data[count])
            return k_recents
