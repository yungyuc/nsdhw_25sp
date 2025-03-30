import pytest
import _matrix
import time


def test_matrix_basic():
    mat = _matrix.Matrix(3, 3)

    mat[0, 0] = 1.0
    mat[0, 1] = 2.0
    mat[1, 1] = 3.0

    assert mat[0, 0] == 1.0
    assert mat[0, 1] == 2.0
    assert mat[1, 1] == 3.0
    assert mat[2, 2] == 0.0
    assert mat.nrow == 3
    assert mat.ncol == 3


def test_matrix_equality():
    mat1 = _matrix.Matrix(2, 2)
    mat2 = _matrix.Matrix(2, 2)

    mat1[0, 0] = 1.0
    mat1[0, 1] = 2.0
    mat1[1, 0] = 3.0
    mat1[1, 1] = 4.0

    mat2[0, 0] = 1.0
    mat2[0, 1] = 2.0
    mat2[1, 0] = 3.0
    mat2[1, 1] = 4.0

    assert mat1 == mat2

    mat2[1, 1] = 5.0
    assert mat1 != mat2


def test_matrix_multiply_naive():
    mat1 = _matrix.Matrix(2, 3)
    mat2 = _matrix.Matrix(3, 2)

    mat1[0, 0] = 1.0
    mat1[0, 1] = 2.0
    mat1[0, 2] = 3.0
    mat1[1, 0] = 4.0
    mat1[1, 1] = 5.0
    mat1[1, 2] = 6.0

    mat2[0, 0] = 7.0
    mat2[0, 1] = 8.0
    mat2[1, 0] = 9.0
    mat2[1, 1] = 10.0
    mat2[2, 0] = 11.0
    mat2[2, 1] = 12.0

    result = _matrix.multiply_naive(mat1, mat2)

    assert result.nrow == 2
    assert result.ncol == 2
    assert result[0, 0] == 58.0  # 1*7 + 2*9 + 3*11
    assert result[0, 1] == 64.0  # 1*8 + 2*10 + 3*12
    assert result[1, 0] == 139.0  # 4*7 + 5*9 + 6*11
    assert result[1, 1] == 154.0  # 4*8 + 5*10 + 6*12


def test_matrix_multiply_tile():
    mat1 = _matrix.Matrix(2, 3)
    mat2 = _matrix.Matrix(3, 2)

    mat1[0, 0] = 1.0
    mat1[0, 1] = 2.0
    mat1[0, 2] = 3.0
    mat1[1, 0] = 4.0
    mat1[1, 1] = 5.0
    mat1[1, 2] = 6.0

    mat2[0, 0] = 7.0
    mat2[0, 1] = 8.0
    mat2[1, 0] = 9.0
    mat2[1, 1] = 10.0
    mat2[2, 0] = 11.0
    mat2[2, 1] = 12.0

    result = _matrix.multiply_tile(mat1, mat2, 2)

    assert result.nrow == 2
    assert result.ncol == 2
    assert result[0, 0] == 58.0
    assert result[0, 1] == 64.0
    assert result[1, 0] == 139.0
    assert result[1, 1] == 154.0


def test_matrix_multiply_mkl():
    mat1 = _matrix.Matrix(2, 3)
    mat2 = _matrix.Matrix(3, 2)

    mat1[0, 0] = 1.0
    mat1[0, 1] = 2.0
    mat1[0, 2] = 3.0
    mat1[1, 0] = 4.0
    mat1[1, 1] = 5.0
    mat1[1, 2] = 6.0

    mat2[0, 0] = 7.0
    mat2[0, 1] = 8.0
    mat2[1, 0] = 9.0
    mat2[1, 1] = 10.0
    mat2[2, 0] = 11.0
    mat2[2, 1] = 12.0

    result = _matrix.multiply_mkl(mat1, mat2)

    assert result.nrow == 2
    assert result.ncol == 2
    assert result[0, 0] == 58.0
    assert result[0, 1] == 64.0
    assert result[1, 0] == 139.0
    assert result[1, 1] == 154.0


def test_multiplication_consistency():
    size = 100
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)

    for i in range(size):
        for j in range(size):
            mat1[i, j] = i * size + j + 1
            mat2[i, j] = i * size + j + 1

    result_naive = _matrix.multiply_naive(mat1, mat2)
    result_tile = _matrix.multiply_tile(mat1, mat2, 16)
    result_mkl = _matrix.multiply_mkl(mat1, mat2)

    for i in range(size):
        for j in range(size):
            assert result_naive[i, j] == result_tile[i, j]
            assert result_naive[i, j] == result_mkl[i, j]


def test_performance():
    size = 500

    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)

    for i in range(size):
        for j in range(size):
            mat1[i, j] = i * size + j + 1
            mat2[i, j] = i * size + j + 1

    # test naive implementation
    start = time.time()
    _matrix.multiply_naive(mat1, mat2)
    naive_time = time.time() - start

    # test tiling implementation
    start = time.time()
    _matrix.multiply_tile(mat1, mat2, 16)
    tile_time = time.time() - start

    # test MKL implementation
    start = time.time()
    _matrix.multiply_mkl(mat1, mat2)
    mkl_time = time.time() - start

    # check if tile is 20% faster
    assert tile_time < naive_time * \
        0.8, f"Tile is not 20% faster: naive={naive_time}, tile={tile_time}"
    print(
        f"Performance: naive={naive_time:.4f}s, tile={tile_time:.4f}s, mkl={mkl_time:.4f}s")
    print(
        f"Tile speedup: {naive_time/tile_time:.2f}x, MKL speedup: {naive_time/mkl_time:.2f}x")


if __name__ == "__main__":
    pytest.main(["-xvs", __file__])
