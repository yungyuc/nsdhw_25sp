from _vector import calculate_angle
import math


def test_180deg():
    result = calculate_angle(-1,0,1,0)
    expected = 3.141592653589793
    rel_tol = 1e-5
    assert math.isclose(result, expected)
    

def test_0deg():
    result = calculate_angle(-1,0,-1,0)
    expected = 0
    rel_tol = 1e-5
    assert math.isclose(result, expected)

