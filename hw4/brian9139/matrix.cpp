// matrix.cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <cblas.h>
#include <atomic>
#include <cstddef>
#include <chrono>

// 全域計數器
static std::atomic<size_t> g_bytes_in_use{0};
static std::atomic<size_t> g_total_allocated{0};
static std::atomic<size_t> g_total_deallocated{0};

// 自訂 allocator
template<typename T>
struct CustomAllocator {
    using value_type = T;
    CustomAllocator() = default;
    template<typename U> CustomAllocator(const CustomAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        size_t bytes = n * sizeof(T);
        g_bytes_in_use += bytes;
        g_total_allocated += bytes;
        return static_cast<T*>(::operator new(bytes));
    }
    void deallocate(T* p, std::size_t n) noexcept {
        size_t bytes = n * sizeof(T);
        g_bytes_in_use -= bytes;
        g_total_deallocated += bytes;
        ::operator delete(p);
    }
};
template<typename A, typename B>
bool operator==(const CustomAllocator<A>&, const CustomAllocator<B>&) { return true; }
template<typename A, typename B>
bool operator!=(const CustomAllocator<A>&, const CustomAllocator<B>&) { return false; }

// Matrix 類別
class Matrix {
public:
    int rows_, cols_;
    std::vector<double, CustomAllocator<double>> data_;
    Matrix(int rows, int cols)
        : rows_(rows), cols_(cols), data_(rows * cols, 0.0) {}
    double& operator()(int i, int j)       { return data_[i * cols_ + j]; }
    double  operator()(int i, int j) const { return data_[i * cols_ + j]; }
};

extern "C" {
    typedef void* MatrixHandle;

    MatrixHandle create_matrix(int rows, int cols) {
        return new Matrix(rows, cols);
    }
    void free_matrix(MatrixHandle h) {
        delete reinterpret_cast<Matrix*>(h);
    }
    void randomize_matrix_c(MatrixHandle h) {
        Matrix* M = reinterpret_cast<Matrix*>(h);
        for (size_t i = 0; i < M->data_.size(); i++) {
            M->data_[i] = static_cast<double>(rand()) / RAND_MAX;
        }
    }
    int matrix_rows(MatrixHandle h) {
        return reinterpret_cast<Matrix*>(h)->rows_;
    }
    int matrix_cols(MatrixHandle h) {
        return reinterpret_cast<Matrix*>(h)->cols_;
    }

    MatrixHandle multiply_naive_c(MatrixHandle Ah, MatrixHandle Bh) {
        Matrix* A = reinterpret_cast<Matrix*>(Ah);
        Matrix* B = reinterpret_cast<Matrix*>(Bh);
        int m = A->rows_, k = A->cols_, n = B->cols_;
        Matrix* C = new Matrix(m, n);
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j) {
                double sum = 0.0;
                for (int p = 0; p < k; ++p)
                    sum += (*A)(i, p) * (*B)(p, j);
                (*C)(i, j) = sum;
            }
        return reinterpret_cast<MatrixHandle>(C);
    }

    MatrixHandle multiply_tile_c(MatrixHandle Ah, MatrixHandle Bh, int tile_size) {
        Matrix* A = reinterpret_cast<Matrix*>(Ah);
        Matrix* B = reinterpret_cast<Matrix*>(Bh);
        int m = A->rows_, k = A->cols_, n = B->cols_;
        Matrix* C = new Matrix(m, n);
        std::fill(C->data_.begin(), C->data_.end(), 0.0);
        for (int i0 = 0; i0 < m; i0 += tile_size) {
            int i_max = std::min(i0 + tile_size, m);
            for (int k0 = 0; k0 < k; k0 += tile_size) {
                int k_max = std::min(k0 + tile_size, k);
                for (int j0 = 0; j0 < n; j0 += tile_size) {
                    int j_max = std::min(j0 + tile_size, n);
                    for (int i = i0; i < i_max; ++i)
                        for (int p = k0; p < k_max; ++p) {
                            double a_ip = (*A)(i, p);
                            for (int j = j0; j < j_max; ++j)
                                (*C)(i, j) += a_ip * (*B)(p, j);
                        }
                }
            }
        }
        return reinterpret_cast<MatrixHandle>(C);
    }

    // 在 NO_BLAS 環境下改用 tiled fallback，否則呼叫 cblas_dgemm
    MatrixHandle multiply_mkl_c(MatrixHandle Ah, MatrixHandle Bh) {
#ifdef NO_BLAS
        // tile fallback, tile size chosen as 64
        return multiply_tile_c(Ah, Bh, 64);
#else
        Matrix* A = reinterpret_cast<Matrix*>(Ah);
        Matrix* B = reinterpret_cast<Matrix*>(Bh);
        int m = A->rows_, k = A->cols_, n = B->cols_;
        Matrix* C = new Matrix(m, n);
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    m, n, k,
                    1.0,
                    A->data_.data(), k,
                    B->data_.data(), n,
                    0.0,
                    C->data_.data(), n);
        return reinterpret_cast<MatrixHandle>(C);
#endif
    }

    int compare_matrices_c(MatrixHandle Ah, MatrixHandle Bh, double tol) {
        Matrix* A = reinterpret_cast<Matrix*>(Ah);
        Matrix* B = reinterpret_cast<Matrix*>(Bh);
        if (A->rows_ != B->rows_ || A->cols_ != B->cols_) return 0;
        for (size_t i = 0; i < A->data_.size(); ++i)
            if (std::abs(A->data_[i] - B->data_[i]) > tol)
                return 0;
        return 1;
    }

    double get_element(void* h, int i, int j) {
        return (*reinterpret_cast<Matrix*>(h))(i, j);
    }
    void set_element(void* h, int i, int j, double v) {
        (*reinterpret_cast<Matrix*>(h))(i, j) = v;
    }

    size_t matrix_bytes()      { return g_bytes_in_use.load(); }
    size_t matrix_allocated()  { return g_total_allocated.load(); }
    size_t matrix_deallocated(){ return g_total_deallocated.load(); }
}

// 主程式
int main(int argc, char** argv) {
    srand((unsigned)time(NULL));
    using clk = std::chrono::high_resolution_clock;

    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        int m = 100;
        void* A = create_matrix(m, m);
        void* B = create_matrix(m, m);
        randomize_matrix_c(A);
        randomize_matrix_c(B);
        void* C1 = multiply_naive_c(A, B);
        void* C2 = multiply_tile_c(A, B, 64);
        void* C3 = multiply_mkl_c(A, B);
        bool ok = compare_matrices_c(C1, C2, 1e-6) && compare_matrices_c(C1, C3, 1e-6);
        if (!ok) {
            std::cerr << "Error: results differ\n";
            return 1;
        }
        std::cout << "Unit tests passed.\n";
        free_matrix(C1);
        free_matrix(C2);
        free_matrix(C3);
        free_matrix(A);
        free_matrix(B);
        return 0;
    } else {
        int n = 1000;
        void* A = create_matrix(n, n);
        void* B = create_matrix(n, n);
        randomize_matrix_c(A);
        randomize_matrix_c(B);
        auto t0 = clk::now();
        void* C1 = multiply_naive_c(A, B);
        auto t1 = clk::now();
        void* C2 = multiply_tile_c(A, B, 64);
        auto t2 = clk::now();
        void* C3 = multiply_mkl_c(A, B);
        auto t3 = clk::now();
        FILE* fp = fopen("performance.txt", "w");
        if (fp) {
            fprintf(fp, "Matrix size: %d x %d\n", n, n);
            fprintf(fp, "Naive:  %f s\n", std::chrono::duration<double>(t1 - t0).count());
            fprintf(fp, "Tile:   %f s\n", std::chrono::duration<double>(t2 - t1).count());
            fprintf(fp, "MKL:    %f s\n", std::chrono::duration<double>(t3 - t2).count());
            fclose(fp);
        }
        free_matrix(C1);
        free_matrix(C2);
        free_matrix(C3);
        free_matrix(A);
        free_matrix(B);
        std::cout << "Performance results written to performance.txt\n";
        return 0;
    }
}
