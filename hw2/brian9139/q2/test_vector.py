import pytest
import _vector
import math

def test_vector_angle():
    assert math.isclose(_vector.vector_angle(1, 0, 0, 1), math.pi / 2, rel_tol=1e-5)
    assert math.isclose(_vector.vector_angle(1, 0, 1, 0), 0.0, rel_tol=1e-5)
    assert math.isclose(_vector.vector_angle(1, 0, -1, 0), math.pi, rel_tol=1e-5)

    with pytest.raises(ValueError):
        _vector.vector_angle(0, 0, 1, 1)  # 零向量應該拋出錯誤
