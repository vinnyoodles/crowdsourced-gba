import unittest
import ringbuffer

class TestRingBuffer(unittest.TestCase):
    buf = ringbuffer.RingBuffer(3)
    def setUp(self):
        pass

    def test_append(self):
        TestRingBuffer.buf.append(1)
        self.assertEquals(len(TestRingBuffer.buf._data), 1)
        TestRingBuffer.buf.append(2)
        self.assertEquals(len(TestRingBuffer.buf._data), 2)
        TestRingBuffer.buf.append(3)
        self.assertEquals(len(TestRingBuffer.buf._data), 3)

    def test_get_k_recent(self):
        self.assertEquals(TestRingBuffer.buf.get_k_recent(3), 3)
        TestRingBuffer.buf.append(100)
        self.assertEquals(TestRingBuffer.buf.get_k_recent(4), 100)
        self.assertEquals(TestRingBuffer.buf.get_k_recent(0), 'Invalid Lookup')


if __name__ == '__main__':
    unittest.main()
