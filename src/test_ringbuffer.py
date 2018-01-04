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
        self.assertEquals(TestRingBuffer.buf.get_k_recent(3), [3, 2, 1])
        TestRingBuffer.buf.append(100)
        self.assertEquals(TestRingBuffer.buf.get_k_recent(4), [100, 3 , 2])

        with self.assertRaises(Exception) as context:
            TestRingBuffer.buf.get_k_recent(-1)
        self.assertTrue('Invalid Lookup' in context.exception)

if __name__ == '__main__':
    unittest.main()
