#!/usr/bin/env python3

import unittest
import timeit
import sys
import os
import pytest

# Import C++ module for testing
import _matrix

# Ensure support for both unittest and pytest
class TestMatrix(unittest.TestCase):
    
    def setUp(self):
        # Create matrices for testing
        self.size = 100
        self.mat1 = _matrix.Matrix(self.size, self.size)
        self.mat2 = _matrix.Matrix(self.size, self.size)
        self.mat_zero = _matrix.Matrix(self.size, self.size)
        
        # Initialize matrices
        for i in range(self.size):
            for j in range(self.size):
                self.mat1[i, j] = i * self.size + j + 1
                self.mat2[i, j] = i * self.size + j + 1
                # mat_zero defaults to 0
    
    def test_matrix_basic(self):
        """Test basic matrix properties and access"""
        # Test matrix dimensions
        self.assertEqual(self.size, self.mat1.nrow)
        self.assertEqual(self.size, self.mat1.ncol)
        
        # Test matrix element access
        self.assertEqual(1, self.mat1[0, 0])
        self.assertEqual(2, self.mat1[0, 1])
        self.assertEqual(self.size+1, self.mat1[1, 0])
        
        # Test matrix equality
        self.assertEqual(self.mat1, self.mat2)
        self.assertNotEqual(self.mat1, self.mat_zero)
    
    def test_naive_multiply(self):
        """Test naive matrix multiplication"""
        result = _matrix.multiply_naive(self.mat1, self.mat2)
        # Simple check of result matrix dimensions
        self.assertEqual(self.size, result.nrow)
        self.assertEqual(self.size, result.ncol)
        # Verify some elements in result are non-zero
        self.assertNotEqual(0, result[0, 0])
    
    def test_tile_multiply(self):
        """Test tiled matrix multiplication"""
        # Test different tile sizes
        for tile_size in [16, 32, 64]:
            result = _matrix.multiply_tile(self.mat1, self.mat2, tile_size)
            self.assertEqual(self.size, result.nrow)
            self.assertEqual(self.size, result.ncol)
            self.assertNotEqual(0, result[0, 0])
    
    def test_mkl_multiply(self):
        """Test MKL matrix multiplication"""
        result = _matrix.multiply_mkl(self.mat1, self.mat2)
        self.assertEqual(self.size, result.nrow)
        self.assertEqual(self.size, result.ncol)
        self.assertNotEqual(0, result[0, 0])
    
    def test_result_consistency(self):
        """Ensure consistency between different multiplication methods"""
        result_naive = _matrix.multiply_naive(self.mat1, self.mat2)
        result_tile = _matrix.multiply_tile(self.mat1, self.mat2, 32)
        result_mkl = _matrix.multiply_mkl(self.mat1, self.mat2)
        
        # Check if all result matrices are consistent
        for i in range(self.size):
            for j in range(self.size):
                self.assertAlmostEqual(result_naive[i, j], result_tile[i, j], places=6)
                self.assertAlmostEqual(result_naive[i, j], result_mkl[i, j], places=6)
    
    def test_zero_multiply(self):
        """Test multiplication with zero matrix"""
        # Multiply with zero matrix using naive algorithm
        result_naive = _matrix.multiply_naive(self.mat1, self.mat_zero)
        # Verify result is zero matrix
        for i in range(self.size):
            for j in range(self.size):
                self.assertEqual(0, result_naive[i, j])
        
        # Multiply with zero matrix using MKL
        result_mkl = _matrix.multiply_mkl(self.mat1, self.mat_zero)
        # Verify result is zero matrix
        for i in range(self.size):
            for j in range(self.size):
                self.assertEqual(0, result_mkl[i, j])

def benchmark(output_file='performance.txt'):
    """Measure matrix multiplication performance and output results to file"""
    size = 1000
    setup_code = '''
import _matrix

# Create matrices
size = 1000
mat1 = _matrix.Matrix(size, size)
mat2 = _matrix.Matrix(size, size)

# Initialize matrices
for i in range(size):
    for j in range(size):
        mat1[i, j] = i * size + j + 1
        mat2[i, j] = i * size + j + 1
'''
    
    # Test code
    naive_code = '_matrix.multiply_naive(mat1, mat2)'
    tile_code_16 = '_matrix.multiply_tile(mat1, mat2, 16)'
    tile_code_32 = '_matrix.multiply_tile(mat1, mat2, 32)'
    tile_code_64 = '_matrix.multiply_tile(mat1, mat2, 64)'
    mkl_code = '_matrix.multiply_mkl(mat1, mat2)'
    
    # Number of repetitions
    repeat = 3
    
    # Measure performance
    print("Measuring naive matrix multiplication performance...")
    naive_time = min(timeit.Timer(naive_code, setup=setup_code).repeat(repeat=repeat, number=1))
    
    print("Measuring tiled matrix multiplication (tile size=16) performance...")
    tile_time_16 = min(timeit.Timer(tile_code_16, setup=setup_code).repeat(repeat=repeat, number=1))
    
    print("Measuring tiled matrix multiplication (tile size=32) performance...")
    tile_time_32 = min(timeit.Timer(tile_code_32, setup=setup_code).repeat(repeat=repeat, number=1))
    
    print("Measuring tiled matrix multiplication (tile size=64) performance...")
    tile_time_64 = min(timeit.Timer(tile_code_64, setup=setup_code).repeat(repeat=repeat, number=1))
    
    print("Measuring MKL matrix multiplication performance...")
    mkl_time = min(timeit.Timer(mkl_code, setup=setup_code).repeat(repeat=repeat, number=1))
    
    # Calculate speedup
    best_tile_time = min(tile_time_16, tile_time_32, tile_time_64)
    best_tile_size = [16, 32, 64][
        [tile_time_16, tile_time_32, tile_time_64].index(best_tile_time)]
    
    naive_to_tile = naive_time / best_tile_time
    naive_to_mkl = naive_time / mkl_time
    tile_to_mkl = best_tile_time / mkl_time
    
    # Output results to terminal and file
    output = f'''Matrix Multiplication Performance Test Results (Matrix size: {size}x{size}, Repetitions: {repeat})
--------------------------------------------------------------
Naive matrix multiplication time: {naive_time:.6f} seconds
Tiled matrix multiplication time (tile size=16): {tile_time_16:.6f} seconds
Tiled matrix multiplication time (tile size=32): {tile_time_32:.6f} seconds
Tiled matrix multiplication time (tile size=64): {tile_time_64:.6f} seconds
Best tiled matrix multiplication time (tile size={best_tile_size}): {best_tile_time:.6f} seconds
MKL matrix multiplication time: {mkl_time:.6f} seconds
--------------------------------------------------------------
Speedup of tiled vs naive: {naive_to_tile:.2f}x
Speedup of MKL vs naive: {naive_to_mkl:.2f}x
Speedup of MKL vs tiled: {tile_to_mkl:.2f}x
Performance improvement of tiled vs naive: {(1 - best_tile_time/naive_time) * 100:.2f}%
--------------------------------------------------------------
Time ratio of tiled vs naive: {best_tile_time/naive_time:.4f} (required < 0.8)
'''
    
    print(output)
    
    # Write results to file
    with open(output_file, 'w') as f:
        f.write(output)

if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == '--benchmark':
        benchmark()
    else:
        unittest.main() 