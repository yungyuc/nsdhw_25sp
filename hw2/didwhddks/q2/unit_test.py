import pytest
import math
import _vector

def test_vector_angle():
    assert math.isclose(_vector.calc_angle(1, 0, 0, 1), math.pi / 2, rel_tol=1e-6)

    assert math.isclose(_vector.calc_angle(1, 1, -1, -1), math.pi, rel_tol=1e-6)

    assert math.isclose(_vector.calc_angle(1, 0, 1, 0), 0.0, rel_tol=1e-6)

    with pytest.raises(ValueError):
        _vector.calc_angle(0, 0, 0, 0)

if __name__ == "__main__":
    pytest.main()