import os
import ctypes
import pytest

# 載入共享庫 libmatrix.so
lib_path = os.path.join(os.path.dirname(__file__), "libmatrix.so")
lib = ctypes.CDLL(lib_path)

# 定義 C 接口的引數與回傳型態
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

lib.multiply_naive_c.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
lib.multiply_naive_c.restype = ctypes.c_void_p

# 注意：更新 multiply_tile_c 為三個參數
lib.multiply_tile_c.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_int]
lib.multiply_tile_c.restype = ctypes.c_void_p

lib.multiply_mkl_c.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
lib.multiply_mkl_c.restype = ctypes.c_void_p

lib.compare_matrices_c.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_double]
lib.compare_matrices_c.restype = ctypes.c_int

def test_multiplication():
    rows, cols = 50, 50  # 測試用較小矩陣
    A = lib.create_matrix(rows, cols)
    B = lib.create_matrix(rows, cols)
    lib.randomize_matrix_c(A)
    lib.randomize_matrix_c(B)

    C_naive = lib.multiply_naive_c(A, B)
    C_tile = lib.multiply_tile_c(A, B, 64)  # 傳入 tile_size 參數
    C_mkl = lib.multiply_mkl_c(A, B)

    tol = 1e-6
    equal_naive_tile = lib.compare_matrices_c(C_naive, C_tile, tol)
    equal_naive_mkl = lib.compare_matrices_c(C_naive, C_mkl, tol)

    # 釋放所有建立的矩陣
    lib.free_matrix(A)
    lib.free_matrix(B)
    lib.free_matrix(C_naive)
    lib.free_matrix(C_tile)
    lib.free_matrix(C_mkl)

    assert equal_naive_tile == 1, "Naive and Tiled results differ."
    assert equal_naive_mkl == 1, "Naive and MKL results differ."

if __name__ == "__main__":
    pytest.main()
