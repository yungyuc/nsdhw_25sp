#!/usr/bin/env python3
import _matrix
import pytest

def make_sequential_matrix(nrow, ncol):
    mat = _matrix.Matrix(nrow, ncol)
    value = 1
    for i in range(nrow):
        for j in range(ncol):
            mat[i, j] = value
            value += 1
    return mat

def compute_expected_product(A, B):
    n = A.nrow
    m = A.ncol
    p = B.ncol
    result = [[0.0 for _ in range(p)] for _ in range(n)]
    for i in range(n):
        for j in range(p):
            for k in range(m):
                result[i][j] += A[i, k] * B[k, j]
    return result

def matrices_are_close(mat_obj, expected, tol=1e-6):
    nrow, ncol = mat_obj.nrow, mat_obj.ncol
    for i in range(nrow):
        for j in range(ncol):
            if abs(mat_obj[i, j] - expected[i][j]) > tol:
                return False
    return True


def test_matrix_basic():
    A = make_sequential_matrix(2, 3)
    assert A.nrow == 2
    assert A.ncol == 3
    
    assert A[0, 0] == 1
    assert A[0, 1] == 2
    assert A[0, 2] == 3
    assert A[1, 0] == 4
    assert A[1, 1] == 5
    assert A[1, 2] == 6

def test_matrix_equality():
    A = make_sequential_matrix(3, 3)
    B = make_sequential_matrix(3, 3)
    assert A == B
    B[0, 0] = -1
    assert A != B

def test_multiply_naive_correctness():
    A = make_sequential_matrix(3, 3)
    B = make_sequential_matrix(3, 3)
    result = _matrix.multiply_naive(A, B)
    expected = compute_expected_product(A, B)
    assert matrices_are_close(result, expected)

def test_multiply_tile_consistency():
    A = make_sequential_matrix(3, 3)
    B = make_sequential_matrix(3, 3)
    result_naive = _matrix.multiply_naive(A, B)
    result_tile = _matrix.multiply_tile(A, B, 2)
    for i in range(A.nrow):
        for j in range(B.ncol):
            assert abs(result_naive[i, j] - result_tile[i, j]) < 1e-6

def test_multiply_mkl_consistency():
    A = make_sequential_matrix(3, 3)
    B = make_sequential_matrix(3, 3)
    result_naive = _matrix.multiply_naive(A, B)
    result_mkl = _matrix.multiply_mkl(A, B)
    for i in range(A.nrow):
        for j in range(B.ncol):
            assert abs(result_naive[i, j] - result_mkl[i, j]) < 1e-6

if __name__ == "__main__":
    pytest.main()
