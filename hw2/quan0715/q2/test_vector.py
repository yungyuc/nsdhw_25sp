import pytest
import math
import _vector

def test_right_angle():
    # 測試直角（90度）
    # 向量 (1,0) 和 (0,1)
    angle = _vector.angle(1.0, 0.0, 0.0, 1.0)
    assert math.isclose(angle, math.pi/2, abs_tol=1e-10)

def test_zero_angle():
    # 測試零角度
    # 向量 (3,4) 和 (3,4)
    angle = _vector.angle(3.0, 4.0, 3.0, 4.0)
    assert math.isclose(angle, 0.0, abs_tol=1e-10)

def test_arbitrary_angle():
    # 測試其他角度
    # 向量 (1,1) 和 (1,0) - 45度角
    angle = _vector.angle(1.0, 1.0, 1.0, 0.0)
    assert math.isclose(angle, math.pi/4, abs_tol=1e-10)

def test_zero_length_vector():
    # 測試零長度向量
    # 向量 (0,0) 和 (1,1)
    with pytest.raises(ValueError):
        _vector.angle(0.0, 0.0, 1.0, 1.0)