import pytest
import _vector
import math

def test_vector_angle():
    # 測試基本角度
    assert math.isclose(_vector.vector_angle(1, 0, 0, 1), math.pi / 2, rel_tol=1e-5)  # 90 度
    assert math.isclose(_vector.vector_angle(1, 0, 1, 0), 0.0, rel_tol=1e-5)  # 0 度
    assert math.isclose(_vector.vector_angle(1, 0, -1, 0), math.pi, rel_tol=1e-5)  # 180 度

    # 新增測試案例
    assert math.isclose(_vector.vector_angle(1, 1, 1, 0), math.pi / 4, rel_tol=1e-5)  # 45 度
    assert math.isclose(_vector.vector_angle(1, 2, -1, -2), math.pi, rel_tol=1e-5)  # 180 度
    assert math.isclose(_vector.vector_angle(1, 0, -0.5, math.sqrt(3)/2), 120 * math.pi / 180, rel_tol=1e-5)  # 120 度

    # 測試向量長度不同的情況
    assert math.isclose(_vector.vector_angle(2, 0, 0, 1), math.pi / 2, rel_tol=1e-5)  # 90 度
    assert math.isclose(_vector.vector_angle(10, 0, 5, 0), 0.0, rel_tol=1e-5)  # 0 度

    # 測試負座標
    assert math.isclose(_vector.vector_angle(-1, -1, 1, 1), math.pi, rel_tol=1e-5)  # ✅ 兩個向量方向相反，應該是 180°

    # 測試零向量（應該拋出錯誤）
    with pytest.raises(ValueError):
        _vector.vector_angle(0, 0, 1, 1)  # 零向量應該拋出錯誤
