import pytest
import _matrix
import time

def test_matrix_basic():
    # 测试矩阵的基本功能
    mat = _matrix.Matrix(3, 3)
    
    # 测试赋值和读取
    mat[0, 0] = 1.0
    mat[0, 1] = 2.0
    mat[1, 1] = 3.0
    
    assert mat[0, 0] == 1.0
    assert mat[0, 1] == 2.0
    assert mat[1, 1] == 3.0
    assert mat[2, 2] == 0.0
    
    # 测试尺寸
    assert mat.nrow == 3
    assert mat.ncol == 3

def test_matrix_equality():
    # 测试矩阵相等判断
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
    # 测试朴素矩阵乘法
    mat1 = _matrix.Matrix(2, 3)
    mat2 = _matrix.Matrix(3, 2)
    
    # 填充矩阵数据
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
    
    # 检查结果
    assert result.nrow == 2
    assert result.ncol == 2
    assert result[0, 0] == 58.0  # 1*7 + 2*9 + 3*11
    assert result[0, 1] == 64.0  # 1*8 + 2*10 + 3*12
    assert result[1, 0] == 139.0  # 4*7 + 5*9 + 6*11
    assert result[1, 1] == 154.0  # 4*8 + 5*10 + 6*12

def test_matrix_multiply_tile():
    # 测试分块矩阵乘法
    mat1 = _matrix.Matrix(2, 3)
    mat2 = _matrix.Matrix(3, 2)
    
    # 填充矩阵数据
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
    
    # 检查结果
    assert result.nrow == 2
    assert result.ncol == 2
    assert result[0, 0] == 58.0
    assert result[0, 1] == 64.0
    assert result[1, 0] == 139.0
    assert result[1, 1] == 154.0

def test_matrix_multiply_mkl():
    # 测试MKL矩阵乘法
    mat1 = _matrix.Matrix(2, 3)
    mat2 = _matrix.Matrix(3, 2)
    
    # 填充矩阵数据
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
    
    # 检查结果
    assert result.nrow == 2
    assert result.ncol == 2
    assert result[0, 0] == 58.0
    assert result[0, 1] == 64.0
    assert result[1, 0] == 139.0
    assert result[1, 1] == 154.0

def test_multiplication_consistency():
    # 测试三种乘法方法的结果一致性
    size = 100
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)
    
    # 填充矩阵数据
    for i in range(size):
        for j in range(size):
            mat1[i, j] = i * size + j + 1
            mat2[i, j] = i * size + j + 1
    
    result_naive = _matrix.multiply_naive(mat1, mat2)
    result_tile = _matrix.multiply_tile(mat1, mat2, 16)
    result_mkl = _matrix.multiply_mkl(mat1, mat2)
    
    # 检查结果一致性
    for i in range(size):
        for j in range(size):
            assert result_naive[i, j] == result_tile[i, j]
            assert result_naive[i, j] == result_mkl[i, j]

def test_performance():
    # 测试性能比较
    # 注意：这个测试可能会耗时较长
    size = 500  # 使用较小的矩阵进行测试，以加快测试速度
    
    # 创建矩阵
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)
    
    # 填充矩阵数据
    for i in range(size):
        for j in range(size):
            mat1[i, j] = i * size + j + 1
            mat2[i, j] = i * size + j + 1
    
    # 测量朴素版本性能
    start = time.time()
    _matrix.multiply_naive(mat1, mat2)
    naive_time = time.time() - start
    
    # 测量分块版本性能
    start = time.time()
    _matrix.multiply_tile(mat1, mat2, 16)
    tile_time = time.time() - start
    
    # 测量MKL版本性能
    start = time.time()
    _matrix.multiply_mkl(mat1, mat2)
    mkl_time = time.time() - start
    
    # 检查分块版本是否比朴素版本快至少20%
    assert tile_time < naive_time * 0.8, f"Tile is not 20% faster: naive={naive_time}, tile={tile_time}"
    print(f"Performance: naive={naive_time:.4f}s, tile={tile_time:.4f}s, mkl={mkl_time:.4f}s")
    print(f"Tile speedup: {naive_time/tile_time:.2f}x, MKL speedup: {naive_time/mkl_time:.2f}x")

if __name__ == "__main__":
    # 运行所有测试
    pytest.main(["-xvs", __file__])
