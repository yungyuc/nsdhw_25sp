import _vector
import math
import pytest
import random

def test_standard_angles():
    test_cases = [
        ((1.0, 0.0), (0.0, 1.0), math.pi / 2),
        ((1.0, 0.0), (-1.0, 0.0), math.pi),
        ((1.0, 0.0), (1.0, 0.0), 0), 
        ((math.sqrt(3), 1.0), (2.0, 0.0), math.pi / 6),
        ((1.0, 0.0), (2.0, 2.0), math.pi / 4), 
        ((1.0, 0.0), (1.0, math.sqrt(3)), math.pi / 3) 
    ]
    for vec1, vec2, expected_angle in test_cases:
        angle = _vector.vector_angle(vec1, vec2)
        assert abs(angle - expected_angle) < 1e-9



def test_zero_vector():
    zero_vec = (0.0, 0.0)
    non_zero_vec = (random.uniform(1, 10), random.uniform(1, 10))
    
    assert abs(_vector.vector_angle(zero_vec, non_zero_vec) + 1) < 1e-9
    assert abs(_vector.vector_angle(non_zero_vec, zero_vec) + 1) < 1e-9
    assert abs(_vector.vector_angle(zero_vec, zero_vec) + 1) < 1e-9

def test_random_vectors():
    for _ in range(10):
        x1, y1 = random.uniform(-10, 10), random.uniform(-10, 10)
        x2, y2 = random.uniform(-10, 10), random.uniform(-10, 10)
        
        dot_product = x1 * x2 + y1 * y2
        magnitude_a = math.sqrt(x1**2 + y1**2)
        magnitude_b = math.sqrt(x2**2 + y2**2)

        if magnitude_a == 0 or magnitude_b == 0:
            expected_angle = -1  
        else:
            cos_theta = dot_product / (magnitude_a * magnitude_b)
            cos_theta = max(-1, min(1, cos_theta)) 
            expected_angle = math.acos(cos_theta)

        angle = _vector.vector_angle((x1, y1), (x2, y2))
        assert abs(angle - expected_angle) < 1e-9

if __name__ == "__main__":
    test_standard_angles()
    test_zero_vector()
    test_random_vectors()
