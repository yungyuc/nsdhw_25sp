#include "matrix.hpp"
#include <mkl.h>
#include <vector>

// naive matrix multiplication
Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::runtime_error("Matrix dimensions incompatible for multiplication");
    }

    size_t M = mat1.nrow();
    size_t N = mat2.ncol();
    size_t K = mat1.ncol();

    Matrix result(M, N);

    for (size_t i = 0; i < M; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            double sum = 0.0;
            for (size_t k = 0; k < K; ++k)
            {
                sum += mat1(i, k) * mat2(k, j);
            }
            result(i, j) = sum;
        }
    }

    return result;
}

// tiling matrix multiplication
Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, int tile_size)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::runtime_error("Matrix dimensions incompatible for multiplication");
    }

    size_t M = mat1.nrow();
    size_t N = mat2.ncol();
    size_t K = mat1.ncol();

    // initialize the result matrix
    Matrix result(M, N);
    std::vector<double> &res_data = result.data();
    const std::vector<double> &a_data = mat1.data();
    const std::vector<double> &b_data = mat2.data();

    // transpose matrix B for better memory access pattern
    std::vector<double> b_trans(K * N);
    for (size_t k = 0; k < K; ++k)
    {
        for (size_t j = 0; j < N; ++j)
        {
            b_trans[j * K + k] = b_data[k * N + j];
        }
    }

    // tiling loop
    for (size_t i0 = 0; i0 < M; i0 += tile_size)
    {
        size_t i_end = std::min(i0 + tile_size, M);

        for (size_t j0 = 0; j0 < N; j0 += tile_size)
        {
            size_t j_end = std::min(j0 + tile_size, N);

            // initialize the result block to 0
            for (size_t i = i0; i < i_end; ++i)
            {
                for (size_t j = j0; j < j_end; ++j)
                {
                    res_data[i * N + j] = 0.0;
                }
            }

            // tiling loop for the innermost dimension
            for (size_t k0 = 0; k0 < K; k0 += tile_size)
            {
                size_t k_end = std::min(k0 + tile_size, K);

                for (size_t i = i0; i < i_end; ++i)
                {
                    for (size_t j = j0; j < j_end; ++j)
                    {
                        double sum = 0.0;
                        for (size_t k = k0; k < k_end; ++k)
                        {
                            sum += a_data[i * K + k] * b_data[k * N + j];
                        }
                        res_data[i * N + j] += sum;
                    }
                }
            }
        }
    }

    return result;
}

// MKL matrix multiplication
Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::runtime_error("Matrix dimensions incompatible for multiplication");
    }

    size_t M = mat1.nrow();
    size_t N = mat2.ncol();
    size_t K = mat1.ncol();

    Matrix result(M, N);

    // parameters for DGEMM
    double alpha = 1.0;
    double beta = 0.0;

    // call DGEMM
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                M, N, K,
                alpha,
                mat1.data().data(), K, // matrix A with K columns
                mat2.data().data(), N, // matrix B with N columns
                beta,
                result.data().data(), N); // matrix C with N columns

    return result;
}
