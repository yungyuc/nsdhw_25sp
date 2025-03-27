import os
import sys
import pytest
import _matrix

def test_multiply_naive():
    size = 1000
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)

    assert mat1.ncol == mat2.nrow

    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1

    ret_naive = _matrix.multiply_naive(mat1, mat2)
    ret_mkl = _matrix.multiply_mkl(mat1, mat2)

    for i in range(ret_naive.nrow):
        for j in range(ret_naive.ncol):
            assert abs(ret_naive[i, j] - ret_mkl[i, j]) < 1e-6

def test_multiply_tile():
    size = 1000
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)

    assert mat1.ncol == mat2.nrow

    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1

    ret_tile = _matrix.multiply_tile(mat1, mat2, 32)
    ret_mkl = _matrix.multiply_mkl(mat1, mat2)

    for i in range(ret_tile.nrow):
        for j in range(ret_tile.ncol):
            assert abs(ret_tile[i, j] - ret_mkl[i, j]) < 1e-6
    

if __name__ == "__main__":
    pytest.main(sys.argv)