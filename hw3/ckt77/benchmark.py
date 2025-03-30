import _matrix
import timeit
import sys


def run_benchmark():
    size = 1000
    print(f"Starting benchmark with matrix size {size}x{size}")

    # initialize matrices
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)

    # fill matrices with values
    for i in range(size):
        for j in range(size):
            mat1[i, j] = i * size + j + 1
            mat2[i, j] = i * size + j + 1

    print("Matrices initialized, starting performance tests...")

    # test each implementation multiple times and take the best result
    repeat = 3
    print(f"Running each test {repeat} times and taking the best result")

    # naive algorithm
    print("Testing naive implementation...")
    naive_times = timeit.repeat(lambda: _matrix.multiply_naive(
        mat1, mat2), repeat=repeat, number=1)
    naive_time = min(naive_times)
    print(f"Naive implementation: {naive_time:.6f} seconds")

    # test tiling with different tile sizes
    print("Testing tiling implementation with different tile sizes...")
    best_tile_time = float('inf')
    best_tile_size = 0

    for tile_size in [16, 32, 64, 128]:
        print(f"Testing tile size {tile_size}...")
        tile_times = timeit.repeat(lambda: _matrix.multiply_tile(
            mat1, mat2, tile_size), repeat=repeat, number=1)
        tile_time = min(tile_times)
        print(f"Tile size {tile_size}: {tile_time:.6f} seconds")

        if tile_time < best_tile_time:
            best_tile_time = tile_time
            best_tile_size = tile_size

    # re-test with best tile size
    # make sure the best tile size is actually faster than naive
    print(f"Re-testing with best tile size {best_tile_size}...")
    tile_times = timeit.repeat(lambda: _matrix.multiply_tile(
        mat1, mat2, best_tile_size), repeat=repeat, number=1)
    tile_time = min(tile_times)

    # MKL implementation
    print("Testing MKL implementation...")
    mkl_times = timeit.repeat(lambda: _matrix.multiply_mkl(
        mat1, mat2), repeat=repeat, number=1)
    mkl_time = min(mkl_times)
    print(f"MKL implementation: {mkl_time:.6f} seconds")

    tile_speedup = naive_time / tile_time
    mkl_speedup = naive_time / mkl_time
    tile_faster_percent = (1 - tile_time/naive_time) * 100

    # determine if tiling is at least 20% faster than naive
    if tile_time >= naive_time * 0.8:
        print(f"WARNING: Tiling is not 20% faster than naive!")
        print(f"Naive: {naive_time:.6f}s, Tiling: {tile_time:.6f}s")
        print(
            f"Current speedup: {tile_speedup:.2f}x ({tile_faster_percent:.2f}%)")
        print(f"Required: at least 1.25x (20%)")

    # write performance data to file
    with open('performance.txt', 'w') as f:
        f.write(f'Matrix multiplication performance (1000x1000):\n')
        f.write(f'Naive implementation: {naive_time:.6f} seconds\n')
        f.write(
            f'Tiling implementation (tile size {best_tile_size}): {tile_time:.6f} seconds\n')
        f.write(f'MKL implementation: {mkl_time:.6f} seconds\n')
        f.write(f'Tiling speedup over naive: {tile_speedup:.2f}x\n')
        f.write(f'MKL speedup over naive: {mkl_speedup:.2f}x\n')
        f.write(f'Tiling is {tile_faster_percent:.2f}% faster than naive\n')
        f.write(f'Best tile size: {best_tile_size}\n')

    print("Performance data written to performance.txt")

    return tile_time < naive_time * 0.8


if __name__ == "__main__":
    success = run_benchmark()
    if not success:
        print("ERROR: Performance requirements not met!")
        sys.exit(1)
