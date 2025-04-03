import pytest
import _matrix as matrix

def print_m(mat):
    for i in range(mat.nrow):
        for j in range(mat.ncol):
            print(mat[i,j], ', ', end = "")
        print("\n", end = "")

def initialize(mat):
    for i in range(mat.nrow):
        for j in range(mat.ncol):
            if(i%2 == 0):
                mat[i,j] = j
            else:
                mat[i,j] = mat.ncol - 1 - j

def ones(t):
    res = matrix.Matrix(t,t)
    for i in range(res.nrow):
        for j in range(res.ncol):
            if(i==j):
                res[i,j] = 1
    return res
    



def test_matrix_assignment():
    mat = matrix.Matrix(3, 3)

    mat[0, 0] = 1.0
    mat[0, 1] = 2.0
    mat[1, 1] = 3.0

    assert mat[0, 0] == 1.0
    assert mat[0, 1] == 2.0
    assert mat[1, 1] == 3.0
    assert mat[2, 2] == 0.0
    assert mat.nrow == 3
    assert mat.ncol == 3

def test_matrix_naive():
    mat1 = matrix.Matrix(2,2)
    mat2 = matrix.Matrix(2,2)

    mat1[0,0] = 1.0
    mat1[1,1] = 1.0

    mat2[0,0] = 2.0
    mat2[0,1] = 3.0
    mat2[1,0] = 4.0
    mat2[1,1] = 5.0

    res = matrix.multiply_naive(mat1, mat2)

    assert res[0,0] == mat2[0,0]
    assert res[0,1] == mat2[0,1]
    assert res[1,1] == mat2[1,1]
    assert res[1,0] == mat2[1,0]

def test_matrix_mkl():
    mat1 = matrix.Matrix(2,2)
    mat2 = matrix.Matrix(2,2)

    mat1[0,0] = 1.0
    mat1[1,1] = 1.0

    mat2[0,0] = 2.0
    mat2[0,1] = 3.0
    mat2[1,0] = 4.0
    mat2[1,1] = 5.0

    res = matrix.multiply_mkl(mat1, mat2)

    # print("result of multiply_mkl()")
    # print_m(res)

    assert res[0,0] == mat2[0,0]
    assert res[0,1] == mat2[0,1]
    assert res[1,1] == mat2[1,1]
    assert res[1,0] == mat2[1,0]

def test_matrix_tile():
    size =12
    mat1 = matrix.Matrix(size,size)
    initialize(mat1)
    
    mat1 = ones(size)
    mat2 = ones(size)

    res = matrix.Matrix(mat1.nrow, mat2.ncol)

    ref = matrix.multiply_mkl(mat1,mat2)

    res = matrix.multiply_tile(mat1, mat2, 10)

    print("result of multiply_mkl()")
    print_m(ref)
    
    print("result of multiply_tile()")
    print_m(res)
    
    for i in range(ref.nrow):
        for j in range(ref.ncol):
            assert ref[i,j] == res[i,j]




if __name__ == "__main__":
    pytest.main(["-xvs", __file__])

