# _matrix.py
import os
import ctypes

_lib = ctypes.CDLL(os.path.join(os.path.dirname(__file__), "libmatrix.so"))

# C 接口設定
_lib.create_matrix.argtypes = [ctypes.c_int, ctypes.c_int]
_lib.create_matrix.restype = ctypes.c_void_p

_lib.free_matrix.argtypes = [ctypes.c_void_p]
_lib.free_matrix.restype = None

_lib.randomize_matrix_c.argtypes = [ctypes.c_void_p]
_lib.randomize_matrix_c.restype = None

_lib.matrix_rows.argtypes = [ctypes.c_void_p]
_lib.matrix_rows.restype = ctypes.c_int

_lib.matrix_cols.argtypes = [ctypes.c_void_p]
_lib.matrix_cols.restype = ctypes.c_int

_lib.get_element.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
_lib.get_element.restype = ctypes.c_double

_lib.set_element.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_double]
_lib.set_element.restype = None

_lib.multiply_naive_c.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
_lib.multiply_naive_c.restype = ctypes.c_void_p

_lib.multiply_tile_c.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_int]
_lib.multiply_tile_c.restype = ctypes.c_void_p

_lib.multiply_mkl_c.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
_lib.multiply_mkl_c.restype = ctypes.c_void_p

# 記憶體追蹤
_lib.matrix_bytes.argtypes = []
_lib.matrix_bytes.restype = ctypes.c_size_t

_lib.matrix_allocated.argtypes = []
_lib.matrix_allocated.restype = ctypes.c_size_t

_lib.matrix_deallocated.argtypes = []
_lib.matrix_deallocated.restype = ctypes.c_size_t


class Matrix:
    def __init__(self, nrow, ncol):
        self.nrow, self.ncol = nrow, ncol
        self.obj = _lib.create_matrix(nrow, ncol)

    def __del__(self):
        if getattr(self, 'obj', None):
            _lib.free_matrix(self.obj)
            self.obj = None

    def __getitem__(self, key):
        i, j = key
        return _lib.get_element(self.obj, i, j)

    def __setitem__(self, key, value):
        i, j = key
        _lib.set_element(self.obj, i, j, value)

    def __eq__(self, other):
        if not isinstance(other, Matrix): return False
        if self.nrow != other.nrow or self.ncol != other.ncol: return False
        for i in range(self.nrow):
            for j in range(self.ncol):
                if self[i, j] != other[i, j]:
                    return False
        return True

def multiply_naive(A, B):
    ret = _lib.multiply_naive_c(A.obj, B.obj)
    M = Matrix.__new__(Matrix)
    M.nrow, M.ncol = A.nrow, B.ncol
    M.obj = ret
    return M

def multiply_tile(A, B, tile_size):
    ret = _lib.multiply_tile_c(A.obj, B.obj, tile_size)
    M = Matrix.__new__(Matrix)
    M.nrow, M.ncol = A.nrow, B.ncol
    M.obj = ret
    return M

def multiply_mkl(A, B):
    ret = _lib.multiply_mkl_c(A.obj, B.obj)
    M = Matrix.__new__(Matrix)
    M.nrow, M.ncol = A.nrow, B.ncol
    M.obj = ret
    return M

def bytes():
    return _lib.matrix_bytes()

def allocated():
    return _lib.matrix_allocated()

def deallocated():
    return _lib.matrix_deallocated()
