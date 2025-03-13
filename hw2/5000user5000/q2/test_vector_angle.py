import _vector
import math

def test_angle():
    # 測試 0 度角 (相同向量)
    assert math.isclose(_vector.calculate_angle(1, 1, 1, 1), 0, rel_tol=1e-9)

    # 測試 90 度角
    assert math.isclose(_vector.calculate_angle(1, 0, 0, 1), math.pi / 2, rel_tol=1e-9)

    # 測試 180 度角
    assert math.isclose(_vector.calculate_angle(1, 0, -1, 0), math.pi, rel_tol=1e-9)

    # 測試隨機角度
    assert math.isclose(_vector.calculate_angle(1, 2, 2, 4), 0, rel_tol=1e-9)

    # 測試零向量 (應該拋出錯誤)
    try:
        _vector.calculate_angle(0, 0, 1, 1)
        assert False  # 不應該執行到這行
    except ValueError:
        assert True

test_angle()
print("All tests passed!")
