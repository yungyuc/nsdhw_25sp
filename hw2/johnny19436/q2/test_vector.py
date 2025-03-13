import math
import _vector
import pytest

def test_angle_orthogonal():
    # Vectors (1,0) and (0,1) are perpendicular -> angle = pi/2
    result = _vector.angle_between(1, 0, 0, 1)
    assert math.isclose(result, math.pi/2, rel_tol=1e-6)

def test_angle_parallel():
    # Vectors (1,0) and (2,0) are parallel -> angle = 0
    result = _vector.angle_between(1, 0, 2, 0)
    assert math.isclose(result, 0.0, rel_tol=1e-6)

def test_angle_opposite():
    # Vectors (1,0) and (-1,0) are opposite -> angle = pi
    result = _vector.angle_between(1, 0, -1, 0)
    assert math.isclose(result, math.pi, rel_tol=1e-6)

def test_angle_invalid():
    # Using a zero vector should raise an exception.
    with pytest.raises(ValueError):
        _vector.angle_between(0, 0, 1, 1)
