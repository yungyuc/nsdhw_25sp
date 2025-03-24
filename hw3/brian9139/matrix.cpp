#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <cblas.h>

// 二維矩陣類別
class Matrix {
public:
    int rows_, cols_;
    std::vector<double> data_;

    Matrix(int rows, int cols)
        : rows_(rows), cols_(cols), data_(rows * cols, 0.0) {}

    double& operator()(int i, int j) {
        return data_[i * cols_ + j];
    }
    
    double operator()(int i, int j) const {
        return data_[i * cols_ + j];
    }
};

// Naive 三層迴圈矩陣乘法
Matrix multiply_naive(const Matrix &A, const Matrix &B) {
    assert(A.cols_ == B.rows_);
    int m = A.rows_;
    int n = B.cols_;
    int k = A.cols_;
    Matrix C(m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int p = 0; p < k; p++) {
                sum += A(i, p) * B(p, j);
            }
            C(i, j) = sum;
        }
    }
    return C;
}

// Tiled 矩陣乘法，tile_size 為區塊大小
// 改進版 Tiled 矩陣乘法，採用 i-k-j 迴圈順序，tile_size 為區塊大小
Matrix multiply_tile(const Matrix &A, const Matrix &B, int tile_size) {
    assert(A.cols_ == B.rows_);
    int m = A.rows_, n = B.cols_, k = A.cols_;
    Matrix C(m, n);
    // 初始化 C 為零
    std::fill(C.data_.begin(), C.data_.end(), 0.0);
    for (int i0 = 0; i0 < m; i0 += tile_size) {
        int i_max = std::min(i0 + tile_size, m);
        for (int k0 = 0; k0 < k; k0 += tile_size) {
            int k_max = std::min(k0 + tile_size, k);
            for (int j0 = 0; j0 < n; j0 += tile_size) {
                int j_max = std::min(j0 + tile_size, n);
                for (int i = i0; i < i_max; i++) {
                    for (int p = k0; p < k_max; p++) {
                        double a_ip = A(i, p);
                        for (int j = j0; j < j_max; j++) {
                            C(i, j) += a_ip * B(p, j);
                        }
                    }
                }
            }
        }
    }
    return C;
}

// 使用 BLAS DGEMM 的矩陣乘法
Matrix multiply_mkl(const Matrix &A, const Matrix &B) {
    assert(A.cols_ == B.rows_);
    int m = A.rows_;
    int n = B.cols_;
    int k = A.cols_;
    Matrix C(m, n);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                m, n, k,
                1.0,
                A.data_.data(), k,
                B.data_.data(), n,
                0.0,
                C.data_.data(), n);
    return C;
}

// 用亂數填入矩陣
void randomize_matrix(Matrix &M) {
    for (size_t i = 0; i < M.data_.size(); i++) {
        M.data_[i] = static_cast<double>(rand()) / RAND_MAX;
    }
}

// 比較兩矩陣是否相等（容許誤差 tol）
bool matrices_equal(const Matrix &A, const Matrix &B, double tol = 1e-6) {
    if (A.rows_ != B.rows_ || A.cols_ != B.cols_)
        return false;
    for (size_t i = 0; i < A.data_.size(); i++) {
        if (std::abs(A.data_[i] - B.data_[i]) > tol) {
            return false;
        }
    }
    return true;
}

// --- 提供給 Python 封裝使用的 C 接口 ---
extern "C" {

    typedef void* MatrixHandle;

    // 建立新矩陣
    MatrixHandle create_matrix(int rows, int cols) {
        return new Matrix(rows, cols);
    }

    // 釋放矩陣記憶體
    void free_matrix(MatrixHandle handle) {
        Matrix* m = reinterpret_cast<Matrix*>(handle);
        delete m;
    }

    // 將矩陣填入亂數值
    void randomize_matrix_c(MatrixHandle handle) {
        Matrix* m = reinterpret_cast<Matrix*>(handle);
        randomize_matrix(*m);
    }

    // 取得矩陣行數
    int matrix_rows(MatrixHandle handle) {
        Matrix* m = reinterpret_cast<Matrix*>(handle);
        return m->rows_;
    }

    // 取得矩陣列數
    int matrix_cols(MatrixHandle handle) {
        Matrix* m = reinterpret_cast<Matrix*>(handle);
        return m->cols_;
    }

    // Naive 乘法介面
    MatrixHandle multiply_naive_c(MatrixHandle A_handle, MatrixHandle B_handle) {
        Matrix* A = reinterpret_cast<Matrix*>(A_handle);
        Matrix* B = reinterpret_cast<Matrix*>(B_handle);
        Matrix result = multiply_naive(*A, *B);
        Matrix* resPtr = new Matrix(result);
        return reinterpret_cast<MatrixHandle>(resPtr);
    }

    // Tiled 乘法介面，接受 tile_size 參數
    MatrixHandle multiply_tile_c(MatrixHandle A_handle, MatrixHandle B_handle, int tile_size) {
        Matrix* A = reinterpret_cast<Matrix*>(A_handle);
        Matrix* B = reinterpret_cast<Matrix*>(B_handle);
        Matrix result = multiply_tile(*A, *B, tile_size);
        Matrix* resPtr = new Matrix(result);
        return reinterpret_cast<MatrixHandle>(resPtr);
    }

    // MKL DGEMM 乘法介面
    MatrixHandle multiply_mkl_c(MatrixHandle A_handle, MatrixHandle B_handle) {
        Matrix* A = reinterpret_cast<Matrix*>(A_handle);
        Matrix* B = reinterpret_cast<Matrix*>(B_handle);
        Matrix result = multiply_mkl(*A, *B);
        Matrix* resPtr = new Matrix(result);
        return reinterpret_cast<MatrixHandle>(resPtr);
    }

    // 比較兩矩陣是否相等，回傳 1 表示相等，否則 0
    int compare_matrices_c(MatrixHandle A_handle, MatrixHandle B_handle, double tol) {
        Matrix* A = reinterpret_cast<Matrix*>(A_handle);
        Matrix* B = reinterpret_cast<Matrix*>(B_handle);
        return matrices_equal(*A, *B, tol) ? 1 : 0;
    }

    // 取得矩陣指定位置的元素
    double get_element(void* handle, int i, int j) {
        Matrix* m = reinterpret_cast<Matrix*>(handle);
        return (*m)(i, j);
    }

    // 設定矩陣指定位置的元素
    void set_element(void* handle, int i, int j, double value) {
        Matrix* m = reinterpret_cast<Matrix*>(handle);
        (*m)(i, j) = value;
    }
}

// 主函式：傳入 "test" 則執行單元測試，否則執行效能量測
int main(int argc, char** argv) {
    srand(static_cast<unsigned>(time(NULL)));
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        int m = 100;
        Matrix A(m, m);
        Matrix B(m, m);
        randomize_matrix(A);
        randomize_matrix(B);
        Matrix C1 = multiply_naive(A, B);
        Matrix C2 = multiply_tile(A, B, 64); // 測試時預設 tile_size 為 64
        Matrix C3 = multiply_mkl(A, B);
        if (!matrices_equal(C1, C2) || !matrices_equal(C1, C3)) {
            std::cerr << "Error: Multiplication results do not match!" << std::endl;
            return 1;
        } else {
            std::cout << "Unit tests passed." << std::endl;
            return 0;
        }
    } else {
        int n = 1000;
        Matrix A(n, n);
        Matrix B(n, n);
        randomize_matrix(A);
        randomize_matrix(B);

        using namespace std::chrono;
        auto start = high_resolution_clock::now();
        Matrix C_naive = multiply_naive(A, B);
        auto end = high_resolution_clock::now();
        duration<double> elapsed_naive = end - start;

        start = high_resolution_clock::now();
        Matrix C_tile = multiply_tile(A, B, 64);
        end = high_resolution_clock::now();
        duration<double> elapsed_tile = end - start;

        start = high_resolution_clock::now();
        Matrix C_mkl = multiply_mkl(A, B);
        end = high_resolution_clock::now();
        duration<double> elapsed_mkl = end - start;

        if (!matrices_equal(C_naive, C_tile) || !matrices_equal(C_naive, C_mkl)) {
            std::cerr << "Error: Multiplication results do not match!" << std::endl;
            return 1;
        }

        FILE *fp = fopen("performance.txt", "w");
        if (fp) {
            fprintf(fp, "Matrix size: %d x %d\n", n, n);
            fprintf(fp, "Naive multiplication time: %f seconds\n", elapsed_naive.count());
            fprintf(fp, "Tiled multiplication time: %f seconds\n", elapsed_tile.count());
            fprintf(fp, "MKL DGEMM multiplication time: %f seconds\n", elapsed_mkl.count());
            fprintf(fp, "Tiled/Naive speedup: %f%%\n", (elapsed_tile.count() / elapsed_naive.count()) * 100.0);
            fclose(fp);
        } else {
            std::cerr << "Error: Cannot open performance.txt for writing." << std::endl;
        }
        std::cout << "Performance results written to performance.txt" << std::endl;
        return 0;
    }
}
