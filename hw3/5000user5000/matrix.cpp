#include <iostream>
#include <vector>
#include <chrono>
#include <cassert>
#include <algorithm>
#include <random>
#include <fstream>
#include <cmath>

#ifdef USE_MKL
#include <mkl.h>
#else
#include <cblas.h>
#endif

#ifdef BUILD_PYBIND
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;
#endif

// Two-dimensional Matrix class.
#include <algorithm>  // for std::copy

class Matrix {
public:
    // Constructor
    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = new double[nelement](); // 初始化成 0
    }
    
    // Copy constructor (deep copy)
    Matrix(const Matrix& other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        size_t nelement = m_nrow * m_ncol;
        m_buffer = new double[nelement];
        std::copy(other.m_buffer, other.m_buffer + nelement, m_buffer);
    }
    
    // Move constructor (steal the resources)
    Matrix(Matrix&& other) noexcept
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(other.m_buffer)
    {
        other.m_buffer = nullptr;
        other.m_nrow = 0;
        other.m_ncol = 0;
    }
    
    // Copy assignment operator (deep copy)
    Matrix& operator=(const Matrix& other)
    {
        if (this != &other) {
            size_t nelement = other.m_nrow * other.m_ncol;
            double* new_buffer = new double[nelement];
            std::copy(other.m_buffer, other.m_buffer + nelement, new_buffer);
            delete[] m_buffer;
            m_nrow = other.m_nrow;
            m_ncol = other.m_ncol;
            m_buffer = new_buffer;
        }
        return *this;
    }
    
    // Move assignment operator
    Matrix& operator=(Matrix&& other) noexcept
    {
        if (this != &other) {
            delete[] m_buffer;
            m_nrow = other.m_nrow;
            m_ncol = other.m_ncol;
            m_buffer = other.m_buffer;
            other.m_buffer = nullptr;
            other.m_nrow = 0;
            other.m_ncol = 0;
        }
        return *this;
    }
    
    // Destructor
    ~Matrix()
    {
        delete[] m_buffer;
    }
    
    // No bound check.
    double operator()(size_t row, size_t col) const {
        return m_buffer[row * m_ncol + col];
    }
    double& operator()(size_t row, size_t col) {
        return m_buffer[row * m_ncol + col];
    }
    
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    
    // 提供對內部緩衝區的存取函式
    const double* data() const { return m_buffer; }
    double* data() { return m_buffer; }
    
private:
    size_t m_nrow;
    size_t m_ncol;
    double* m_buffer;
};

// Naive matrix multiplication.
Matrix multiply_naive(const Matrix &A, const Matrix &B) {
    assert(A.ncol() == B.nrow());
    Matrix C(A.nrow(), B.ncol());
    for (size_t i = 0; i < A.nrow(); i++) {
        for (size_t j = 0; j < B.ncol(); j++) {
            double sum = 0.0;
            for (size_t k = 0; k < A.ncol(); k++) {
                sum += A(i, k) * B(k, j);
            }
            C(i, j) = sum;
        }
    }
    return C;
}

// Tiled matrix multiplication.
Matrix multiply_tile(const Matrix &A, const Matrix &B,size_t tile_size=32) {
    assert(A.ncol() == B.nrow());
    Matrix C(A.nrow(), B.ncol());
    std::fill(C.data(), C.data() + A.nrow() * B.ncol(), 0.0);
    for (size_t ii = 0; ii < A.nrow(); ii += tile_size) {
        for (size_t jj = 0; jj < B.ncol(); jj += tile_size) {
            for (size_t kk = 0; kk < A.ncol(); kk += tile_size) {
                size_t i_max = std::min(ii + tile_size, A.nrow());
                size_t j_max = std::min(jj + tile_size, B.ncol());
                size_t k_max = std::min(kk + tile_size, A.ncol());
                for (size_t i = ii; i < i_max; i++) {
                    for (size_t j = jj; j < j_max; j++) {
                        for (size_t k = kk; k < k_max; k++) {
                            C(i, j) += A(i, k) * B(k, j);
                        }
                    }
                }
            }
        }
    }
    return C;
}

// Matrix multiplication using DGEMM from BLAS.
Matrix multiply_mkl(const Matrix &A, const Matrix &B) {
    assert(A.ncol() == B.nrow());
    Matrix C(A.nrow(), B.ncol());
    const double alpha = 1.0, beta = 0.0;
    // 使用 row-major 排列。
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        A.nrow(), B.ncol(), A.ncol(),
        alpha,
        A.data(), A.ncol(),
        B.data(), B.ncol(),
        beta,
        C.data(), C.ncol());
    return C;
}

// 產生值介於 [0,1000000] 的隨機矩陣。
Matrix random_matrix(int rows, int cols) {
    Matrix M(rows, cols);
    std::mt19937 gen(42); // 固定種子以確保結果可重現。
    std::uniform_real_distribution<double> dist(0.0, 1000000.0);
    for (int i = 0; i < rows * cols; i++) {
        M.data()[i] = dist(gen);
    }
    return M;
}

//  比較兩個矩陣是否相等。(容許誤差 tol)
bool compare_matrix(const Matrix &A, const Matrix &B, double tol=1e-5) {
    assert(A.nrow() == B.nrow() && A.ncol() == B.ncol());
    size_t n = A.nrow() * A.ncol();
    for (size_t i = 0; i < n; i++) {
        if (std::abs(A.data()[i] - B.data()[i]) > tol){
            printf("diff: %.11f\n", std::abs(A.data()[i] - B.data()[i]));
            return false;
        }
            
    }
    return true;
}

#ifndef BUILD_PYBIND
// main()：進行各乘法例程的效能測試並將結果寫入 performance.txt。
int main() {
    int size = 1024; // 使用 1024 x 1024 的矩陣。
    std::cout << "Generating random matrices of size " << size << "x" << size << std::endl;
    Matrix A = random_matrix(size, size);
    Matrix B = random_matrix(size, size);
    
    std::cout << "Performing naive multiplication..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    Matrix C_naive = multiply_naive(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_naive = end - start;
    std::cout << "> Naive multiplication took " << duration_naive.count() << " seconds." << std::endl;
    
    const size_t tile_size = 32;

    std::cout << "Performing tiled multiplication..." << std::endl;
    start = std::chrono::high_resolution_clock::now();
    Matrix C_tile = multiply_tile(A, B, tile_size);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_tile = end - start;
    std::cout << "> Tiled multiplication took " << duration_tile.count() << " seconds." << std::endl;
    
    std::cout << "Performing MKL multiplication..." << std::endl;
    start = std::chrono::high_resolution_clock::now();
    Matrix C_mkl = multiply_mkl(A, B);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_mkl = end - start;
    std::cout << "> MKL DGEMM multiplication took " << duration_mkl.count() << " seconds." << std::endl;
    
    // 驗證各方法計算結果的相近程度。
    assert(compare_matrix(C_naive, C_tile));
    assert(compare_matrix(C_naive, C_mkl));
    
    // 將效能數據輸出到 performance.txt。
    std::ofstream perf("performance.txt");
    perf << "Matrix size: " << size << "x" << size << "\n";
    perf << "Naive multiplication time: " << duration_naive.count() << " seconds\n";
    perf << "Tiled multiplication time: " << duration_tile.count() << " seconds\n";
    perf << "MKL DGEMM multiplication time: " << duration_mkl.count() << " seconds\n";
    perf << "Speedup of tiled over naive: " << duration_naive.count() / duration_tile.count() << "x\n";
    perf << "Speedup of MKL over naive: " << duration_naive.count() / duration_mkl.count() << "x\n";
    perf.close();
    
    return 0;
}
#endif

#ifdef BUILD_PYBIND
// Pybind11 module 定義，使得 Matrix 類別及各乘法函數可由 Python 呼叫。
PYBIND11_MODULE(_matrix, m) {
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("rows", &Matrix::nrow) // 可選：為相容 .rows()
        .def("cols", &Matrix::ncol)
        .def("__getitem__", [](const Matrix &self, std::pair<int, int> idx) {
            int i = idx.first, j = idx.second;
            if (i < 0 || static_cast<size_t>(i) >= self.nrow() ||
                j < 0 || static_cast<size_t>(j) >= self.ncol())
                throw std::out_of_range("Index out of range");
            return self(i, j);
        })
        .def("__setitem__", [](Matrix &self, std::pair<int, int> idx, double value) {
            int i = idx.first, j = idx.second;
            if (i < 0 || static_cast<size_t>(i) >= self.nrow() ||
                j < 0 || static_cast<size_t>(j) >= self.ncol())
                throw std::out_of_range("Index out of range");
            self(i, j) = value;
        })
        .def("__eq__", [](const Matrix &a, const Matrix &b) {
            if (a.nrow() != b.nrow() || a.ncol() != b.ncol()) return false;
            size_t total = a.nrow() * a.ncol();
            for (size_t i = 0; i < total; ++i) {
                if (std::abs(a.data()[i] - b.data()[i]) > 1e-9) return false;
            }
            return true;
        })        
        .def("__repr__", [](const Matrix &self) {
            return "<Matrix " + std::to_string(self.nrow()) + "x" + std::to_string(self.ncol()) + ">";
        });
        
    m.def("multiply_naive", &multiply_naive, "Naive matrix multiplication");
    m.def("multiply_tile", &multiply_tile,
        "Tiled matrix multiplication with tile size",
        py::arg("A"), py::arg("B"), py::arg("tile_size"));
    m.def("multiply_mkl", &multiply_mkl, "MKL DGEMM multiplication");
    m.def("random_matrix", &random_matrix, "Generate a random matrix with values in [0,1]");
}
#endif