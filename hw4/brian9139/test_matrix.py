# test_matrix.py  
import os
import ctypes
import pytest

lib = ctypes.CDLL(os.path.join(os.path.dirname(__file__), "libmatrix.so"))

lib.create_matrix.argtypes = [ctypes.c_int, ctypes.c_int]
lib.create_matrix.restype = ctypes.c_void_p

lib.free_matrix.argtypes = [ctypes.c_void_p]
lib.free_matrix.restype = None

lib.randomize_matrix_c.argtypes = [ctypes.c_void_p]
lib.randomize_matrix_c.restype = None

lib.matrix_rows.argtypes = [ctypes.c_void_p]
lib.matrix_rows.restype = ctypes.c_int

lib.matrix_cols.argtypes = [ctypes.c_void_p]
lib.matrix_cols.restype = ctypes.c_int

lib.get_element.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
lib.get_element.restype = ctypes.c_double

lib.set_element.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_double]
lib.set_element.restype = None

lib.multiply_naive_c.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
lib.multiply_naive_c.restype = ctypes.c_void_p

lib.multiply_tile_c.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_int]
lib.multiply_tile_c.restype = ctypes.c_void_p

lib.multiply_mkl_c.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
lib.multiply_mkl_c.restype = ctypes.c_void_p

lib.compare_matrices_c.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_double]
lib.compare_matrices_c.restype = ctypes.c_int

lib.matrix_bytes.argtypes = []
lib.matrix_bytes.restype = ctypes.c_size_t
lib.matrix_allocated.argtypes = []
lib.matrix_allocated.restype = ctypes.c_size_t
lib.matrix_deallocated.argtypes = []
lib.matrix_deallocated.restype = ctypes.c_size_t

def test_multiplication():
    rows, cols = 50, 50
    A = lib.create_matrix(rows, cols)
    B = lib.create_matrix(rows, cols)
    lib.randomize_matrix_c(A)
    lib.randomize_matrix_c(B)

    C1 = lib.multiply_naive_c(A, B)
    C2 = lib.multiply_tile_c(A, B, 64)
    C3 = lib.multiply_mkl_c(A, B)

    tol = 1e-6
    assert lib.compare_matrices_c(C1, C2, tol) == 1
    assert lib.compare_matrices_c(C1, C3, tol) == 1

    lib.free_matrix(A)
    lib.free_matrix(B)
    lib.free_matrix(C1)
    lib.free_matrix(C2)
    lib.free_matrix(C3)

def test_allocator_tracking():
    assert lib.matrix_bytes() == 0

    M = lib.create_matrix(10, 10)
    lib.free_matrix(M)

    assert lib.matrix_allocated() >= 10*10*8
    assert lib.matrix_deallocated() >= 10*10*8
    assert lib.matrix_bytes() == 0
