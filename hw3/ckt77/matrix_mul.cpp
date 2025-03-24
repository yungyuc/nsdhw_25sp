#include "matrix.hpp"

// 根据是否有 MKL 支持决定是否包含 MKL 头文件
#if defined(HAS_MKL)
#include <mkl.h>
#endif

// 检查是否支持 OpenMP
#if defined(_OPENMP)
#include <omp.h>
#endif

// 朴素的矩阵乘法实现
Matrix multiply_naive(const Matrix& mat1, const Matrix& mat2) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::runtime_error("Matrix dimensions incompatible for multiplication");
    }

    size_t M = mat1.nrow();
    size_t N = mat2.ncol();
    size_t K = mat1.ncol();
    
    Matrix result(M, N);
    
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < K; ++k) {
                sum += mat1(i, k) * mat2(k, j);
            }
            result(i, j) = sum;
        }
    }
    
    return result;
}

// 分块(tiling)优化的矩阵乘法实现
Matrix multiply_tile(const Matrix& mat1, const Matrix& mat2, int tile_size) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::runtime_error("Matrix dimensions incompatible for multiplication");
    }

    size_t M = mat1.nrow();
    size_t N = mat2.ncol();
    size_t K = mat1.ncol();
    
    // 创建结果矩阵并初始化为0
    Matrix result(M, N);
    std::vector<double>& res_data = result.data();
    const std::vector<double>& a_data = mat1.data();
    const std::vector<double>& b_data = mat2.data();
    
    // 先计算B的转置，提高缓存命中率
    std::vector<double> b_trans(K * N);
    for (size_t k = 0; k < K; ++k) {
        for (size_t j = 0; j < N; ++j) {
            b_trans[j * K + k] = b_data[k * N + j];
        }
    }
    
    // 使用分块优化
#if defined(_OPENMP)
    #pragma omp parallel for
#endif
    for (size_t i0 = 0; i0 < M; i0 += tile_size) {
        size_t i_end = std::min(i0 + tile_size, M);
        
        for (size_t j0 = 0; j0 < N; j0 += tile_size) {
            size_t j_end = std::min(j0 + tile_size, N);
            
            // 初始化这个分块的结果为0
            for (size_t i = i0; i < i_end; ++i) {
                for (size_t j = j0; j < j_end; ++j) {
                    res_data[i * N + j] = 0.0;
                }
            }
            
            // 按块计算 C = A * B
            for (size_t k0 = 0; k0 < K; k0 += tile_size) {
                size_t k_end = std::min(k0 + tile_size, K);
                
                for (size_t i = i0; i < i_end; ++i) {
                    for (size_t k = k0; k < k_end; ++k) {
                        double a_ik = a_data[i * K + k];
                        for (size_t j = j0; j < j_end; ++j) {
                            // 使用转置矩阵B，提高缓存命中率
                            res_data[i * N + j] += a_ik * b_trans[j * K + k];
                        }
                    }
                }
            }
        }
    }
    
    return result;
}

// 使用MKL的DGEMM实现矩阵乘法
Matrix multiply_mkl(const Matrix& mat1, const Matrix& mat2) {
#if defined(HAS_MKL)
    // 使用 MKL 的实现
    if (mat1.ncol() != mat2.nrow()) {
        throw std::runtime_error("Matrix dimensions incompatible for multiplication");
    }

    size_t M = mat1.nrow();
    size_t N = mat2.ncol();
    size_t K = mat1.ncol();
    
    Matrix result(M, N);
    
    double alpha = 1.0;
    double beta = 0.0;
    
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                M, N, K,
                alpha,
                mat1.data().data(), K,
                mat2.data().data(), N,
                beta,
                result.data().data(), N);
    
    return result;
#else
    // 当没有 MKL 时，回退到 tiling 实现
    return multiply_tile(mat1, mat2, 64);
#endif
}
