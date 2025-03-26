#!/usr/bin/env python3
import _matrix
import numpy as np
import time

def make_random_matrix(size, seed=None):
    if seed is not None:
        np.random.seed(seed)
    mat = _matrix.Matrix(size, size)
    data = np.random.rand(size, size) * 100
    for i in range(size):
        for j in range(size):
            mat[i, j] = float(data[i, j])
    return mat

def time_function(func, *args, repeats=3):
    times = []
    result = None
    for _ in range(repeats):
        start = time.perf_counter()
        result = func(*args)
        end = time.perf_counter()
        times.append(end - start)
    return min(times), result

def main():
    size = 1000
    block_size = 64

    print(f"Generating two random {size}x{size} matrices...")
    A = make_random_matrix(size, seed=42)
    B = make_random_matrix(size, seed=43)

    _ = _matrix.multiply_naive(A, B)
    _ = _matrix.multiply_tile(A, B, block_size)
    _ = _matrix.multiply_mkl(A, B)

    print("Timing multiply_naive...")
    time_naive, _ = time_function(_matrix.multiply_naive, A, B)
    print(f"Naive version time: {time_naive:.6f} sec")

    print("Timing multiply_tile (tiling version)...")
    time_tile, _ = time_function(_matrix.multiply_tile, A, B, block_size)
    print(f"Tiled version time: {time_tile:.6f} sec")

    print("Timing multiply_mkl (MKL version)...")
    time_mkl, _ = time_function(_matrix.multiply_mkl, A, B)
    print(f"MKL version time: {time_mkl:.6f} sec")

    tiled_ratio = (time_tile / time_naive) * 100
    mkl_ratio = (time_mkl / time_naive) * 100

    with open("performance.txt", "w") as f:
        f.write("Matrix Multiplication Performance Benchmark\n")
        f.write("=============================================\n")
        f.write(f"Matrix size: {size}x{size}\n")
        f.write(f"Tiling block size: {block_size}\n\n")
        f.write(f"Naive version time: {time_naive:.6f} sec\n")
        f.write(f"Tiled version time: {time_tile:.6f} sec ({tiled_ratio:.2f}% of naive)\n")
        f.write(f"MKL version time:   {time_mkl:.6f} sec ({mkl_ratio:.2f}% of naive)\n\n")

if __name__ == "__main__":
    main()
