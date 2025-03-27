import time
import _matrix

def main():
    size = 1000
    tsize = 32
    round = 5

    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)

    for i in range(size):
        for j in range(size):
            value = i * size + j + 1
            mat1[i, j] = value
            mat2[i, j] = value

    naive_time = float("inf")
    for i in range(round):
        start_naive = time.perf_counter()
        result_naive = _matrix.multiply_naive(mat1, mat2)
        end_naive = time.perf_counter()
        runtime = end_naive - start_naive
        naive_time = min(naive_time, runtime)

    tile_time = float("inf")
    for i in range(round):
        start_tile = time.perf_counter()
        result_tile = _matrix.multiply_tile(mat1, mat2, tsize)
        end_tile = time.perf_counter()
        runtime = end_tile - start_tile
        tile_time = min(tile_time, runtime)

    # Write the results to 'performance.txt'
    with open("performance.txt", "w") as f:
        f.write("Matrix multiplication performance results:\n")
        f.write("Matrix size: {} x {}\n".format(size, size))
        f.write('Test round: {}, and take the minimum time\n'.format(round))
        f.write("Naive multiplication runtime: {:.6f} seconds\n".format(naive_time))
        f.write("Tiled multiplication runtime (tile size {}): {:.6f} seconds\n".format(tsize, tile_time))
        f.write("Runtime of tiled multiplication is {:.2f}% of naive multiplication\n".format(tile_time / naive_time * 100))

if __name__ == "__main__":
    main()