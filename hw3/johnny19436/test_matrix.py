import _matrix
import numpy as np
import pytest
import time

def create_matrix(n, m):
    mat = _matrix.Matrix(n, m)
    for i in range(n):
        for j in range(m):
            mat[i, j] = np.random.rand()
    return mat

def matrices_equal(M1, M2, tol=1e-6):
    if M1.nrow != M2.nrow or M1.ncol != M2.ncol:
        return False
    for i in range(M1.nrow):
        for j in range(M1.ncol):
            if abs(M1[i, j] - M2[i, j]) > tol:
                return False
    return True

def test_multiplication_correctness():
    n = 256
    A = create_matrix(n, n)
    B = create_matrix(n, n)
    
    C_naive = _matrix.multiply_naive(A, B)
    C_tile = _matrix.multiply_tile(A, B, 16)
    C_mkl = _matrix.multiply_mkl(A, B)
    
    assert matrices_equal(C_naive, C_tile)
    assert matrices_equal(C_naive, C_mkl)

def test_performance():
    n = 1000
    A = create_matrix(n, n)
    B = create_matrix(n, n)
    
    start = time.time()
    C_naive = _matrix.multiply_naive(A, B)
    time_naive = time.time() - start
    
    start = time.time()
    C_tile = _matrix.multiply_tile(A, B, 16)
    time_tile = time.time() - start

    start = time.time()
    C_tile = _matrix.multiply_mkl(A, B)
    time_mkl = time.time() - start
    
    assert time_tile < 0.8 * time_naive, f"Tiled time {time_tile} not less than 80% of naive time {time_naive}"
    
    with open("performance.txt", "w") as f:
        f.write(f"Naive: {time_naive:.4f}s\n")
        f.write(f"Tiled: {time_tile:.4f}s\n")
        f.write(f"Mkl: {time_mkl:.4f}s\n")
