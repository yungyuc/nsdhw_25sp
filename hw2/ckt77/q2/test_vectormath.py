import unittest
import math
from _vector import calculate_angle

class TestVectorMath(unittest.TestCase):
    def test_right_angle(self):
        angle = calculate_angle(1, 0, 0, 1)
        self.assertAlmostEqual(angle, math.pi/2)

    def test_zero_angle(self):
        angle = calculate_angle(1, 1, 1, 1)
        self.assertAlmostEqual(angle, 0)

    def test_arbitrary_angle(self):
        angle = calculate_angle(1, 0, 1, 1)
        self.assertAlmostEqual(angle, math.pi/4)

    def test_zero_vector(self):
        # 測試第一個向量為零向量
        with self.assertRaises(ValueError):
            calculate_angle(0, 0, 1, 1)

    def test_zero_vector_second(self):
        # 測試第二個向量為零向量
        with self.assertRaises(ValueError):
            calculate_angle(1, 1, 0, 0)

if __name__ == '__main__':
    unittest.main()
