import numpy as np
import _matrix

def matrix_to_numpy(mat):
    """Convert a _matrix.Matrix to a NumPy array."""
    rows = mat.rows()
    cols = mat.cols()
    arr = np.zeros((rows, cols))
    for i in range(rows):
        for j in range(cols):
            arr[i, j] = mat[i, j]
    return arr

def test_multiplication_correctness():
    # Create two 3x3 matrices with fixed values.
    tile_size = 32
    A = _matrix.Matrix(3, 3)
    B = _matrix.Matrix(3, 3)
    valuesA = [1, 2, 3, 4, 5, 6, 7, 8, 9]
    valuesB = [9, 8, 7, 6, 5, 4, 3, 2, 1]
    for i in range(3):
        for j in range(3):
            A[i, j] = valuesA[i * 3 + j]
            B[i, j] = valuesB[i * 3 + j]
    
    C_naive = _matrix.multiply_naive(A, B)
    C_tile = _matrix.multiply_tile(A, B,tile_size)
    C_mkl = _matrix.multiply_mkl(A, B)
    
    np_naive = matrix_to_numpy(C_naive)
    np_tile = matrix_to_numpy(C_tile)
    np_mkl = matrix_to_numpy(C_mkl)
    
    # Check that the differences are within a small tolerance.
    assert np.allclose(np_naive, np_tile, atol=1e-8), "Tiled multiplication result does not match naive multiplication."
    assert np.allclose(np_naive, np_mkl, atol=1e-8), "MKL multiplication result does not match naive multiplication."

def test_alloc():
    # Test the random_matrix function.
    print("Allocated:", _matrix.allocated())
    print("Deallocated:", _matrix.deallocated())
    print("Current bytes in use:", _matrix.bytes())
    A = _matrix.random_matrix(1024, 1024)
    B = _matrix.random_matrix(1024, 1024)
    C = _matrix.multiply_naive(A, B)
    print("After matrix creation and multiplication:")
    print("Allocated:", _matrix.allocated())
    print("Deallocated:", _matrix.deallocated())
    print("Current bytes in use:", _matrix.bytes())

if __name__ == "__main__":
    test_multiplication_correctness()
    test_alloc()
    # test_random_matrix()
    print("All tests passed!")