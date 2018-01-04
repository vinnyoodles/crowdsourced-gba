import unittest
import ringbuffer

class TestRingBuffer(unittest.TestCase):

    def setUp(self):
        self.ringbuffer = ringbuffer.RingBuffer(3)

    def tearDown(self):
        del self.ringbuffer

    def test_append(self):
        self.ringbuffer.append(1)
        self.assertEquals(len(self.ringbuffer._data), 1)
        self.ringbuffer.append(2)
        self.assertEquals(len(self.ringbuffer._data), 2)
        self.ringbuffer.append(3)
        self.assertEquals(len(self.ringbuffer._data), 3)

    def test_get_k_recent(self):
        self.ringbuffer.append(1)
        self.ringbuffer.append(2)
        self.ringbuffer.append(3)
        self.assertEquals(self.ringbuffer.get_k_recent(3), [3, 2, 1])
        self.ringbuffer.append(100)
        self.assertEquals(self.ringbuffer.get_k_recent(4), [100, 3 , 2])

        with self.assertRaises(Exception) as context:
            self.ringbuffer.get_k_recent(-1)
        self.assertTrue('Invalid Lookup' in context.exception)

if __name__ == '__main__':
    unittest.main()
