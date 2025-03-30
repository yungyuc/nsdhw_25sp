import os
import ctypes

# 載入共享庫，假設 libmatrix.so 與此檔案位於同一目錄
_lib = ctypes.CDLL(os.path.join(os.path.dirname(__file__), "libmatrix.so"))

# 設定各 C 函式的引數與回傳值型態
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

class Matrix:
    def __init__(self, nrow, ncol):
        self.nrow = nrow
        self.ncol = ncol
        self.obj = _lib.create_matrix(nrow, ncol)

    def __del__(self):
        if hasattr(self, 'obj') and self.obj:
            _lib.free_matrix(self.obj)
            self.obj = None

    def __getitem__(self, key):
        i, j = key
        return _lib.get_element(self.obj, i, j)

    def __setitem__(self, key, value):
        i, j = key
        _lib.set_element(self.obj, i, j, value)

    def __eq__(self, other):
        if not isinstance(other, Matrix):
            return False
        if self.nrow != other.nrow or self.ncol != other.ncol:
            return False
        for i in range(self.nrow):
            for j in range(self.ncol):
                if self[i, j] != other[i, j]:
                    return False
        return True

def multiply_naive(matA, matB):
    ret = _lib.multiply_naive_c(matA.obj, matB.obj)
    res = Matrix(matA.nrow, matB.ncol)
    res.obj = ret
    return res

def multiply_tile(matA, matB, tile_size):
    ret = _lib.multiply_tile_c(matA.obj, matB.obj, tile_size)
    res = Matrix(matA.nrow, matB.ncol)
    res.obj = ret
    return res

def multiply_mkl(matA, matB):
    ret = _lib.multiply_mkl_c(matA.obj, matB.obj)
    res = Matrix(matA.nrow, matB.ncol)
    res.obj = ret
    return res
