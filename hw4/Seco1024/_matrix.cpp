#include <iostream>
#include <vector>
#include <algorithm>
#include <cblas.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

template<typename T>
struct CustomAllocator {
    using value_type = T;

    CustomAllocator() noexcept {}
    template<typename U> CustomAllocator(const CustomAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        std::size_t bytes = n * sizeof(T);
        allocated_bytes += bytes;
        allocation_count++;
        return static_cast<T*>(::operator new(bytes));
    }

    void deallocate(T* p, std::size_t n) noexcept {
        std::size_t bytes = n * sizeof(T);
        deallocated_bytes += bytes;
        deallocation_count++;
        ::operator delete(p);
    }

    inline static std::size_t allocated_bytes    = 0;
    inline static std::size_t deallocated_bytes  = 0;
    inline static std::size_t allocation_count   = 0;
    inline static std::size_t deallocation_count = 0;
};

template<typename T, typename U>
bool operator==(const CustomAllocator<T>&, const CustomAllocator<U>&) { return true; }
template<typename T, typename U>
bool operator!=(const CustomAllocator<T>& a, const CustomAllocator<U>& b) { return !(a == b); }

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
    : nrow_(nrow), ncol_(ncol),
      data_(nrow * ncol, 0.0, CustomAllocator<double>()) {}

    size_t nrow() const { return nrow_; }
    size_t ncol() const { return ncol_; }

    double operator()(size_t i, size_t j) const {
        return data_[i * ncol_ + j];
    }
    double& operator()(size_t i, size_t j) {
        return data_[i * ncol_ + j];
    }
    bool operator==(const Matrix &o) const noexcept {
        if (nrow_ != o.nrow_ || ncol_ != o.ncol_) return false;
        for (size_t i = 0; i < nrow_; ++i)
          for (size_t j = 0; j < ncol_; ++j)
            if ((*this)(i,j) != o(i,j))
              return false;
        return true;
    }
    bool operator!=(const Matrix &o) const noexcept {
        return !(*this == o);
    }

    double* data()       { return data_.data(); }
    const double* data() const { return data_.data(); }

private:
    size_t nrow_, ncol_;
    std::vector<double, CustomAllocator<double>> data_;
};

Matrix multiply_naive(const Matrix &A, const Matrix &B) {
    if (A.ncol() != B.nrow())
        throw std::invalid_argument("Incompatible dimensions.");
    Matrix C(A.nrow(), B.ncol());
    for (size_t i = 0; i < A.nrow(); ++i)
        for (size_t j = 0; j < B.ncol(); ++j)
            for (size_t k = 0; k < A.ncol(); ++k)
                C(i,j) += A(i,k) * B(k,j);
    return C;
}

Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t bs = 64) {
    if (A.ncol() != B.nrow())
        throw std::invalid_argument("Incompatible dimensions.");
    Matrix C(A.nrow(), B.ncol());
    size_t M = A.nrow(), N = A.ncol(), P = B.ncol();
    for (size_t ii = 0; ii < M; ii += bs)
      for (size_t jj = 0; jj < P; jj += bs)
        for (size_t kk = 0; kk < N; kk += bs)
          for (size_t i = ii; i < std::min(ii+bs, M); ++i)
            for (size_t j = jj; j < std::min(jj+bs, P); ++j)
              for (size_t k = kk; k < std::min(kk+bs, N); ++k)
                C(i,j) += A(i,k) * B(k,j);
    return C;
}

Matrix multiply_mkl(const Matrix &A, const Matrix &B) {
    if (A.ncol() != B.nrow())
        throw std::invalid_argument("Incompatible dimensions.");
    Matrix C(A.nrow(), B.ncol());
    cblas_dgemm(
      CblasRowMajor, CblasNoTrans, CblasNoTrans,
      A.nrow(), B.ncol(), A.ncol(),
      1.0, A.data(), A.ncol(),
      B.data(), B.ncol(),
      0.0, C.data(), C.ncol()
    );
    return C;
}

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "matrix multiplication with tracking allocator";

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile, py::arg("A"), py::arg("B"), py::arg("block_size") = 64);
    m.def("multiply_mkl",   &multiply_mkl);

    py::class_<Matrix>(m, "Matrix")
      .def(py::init<size_t, size_t>())
      .def("__eq__", &Matrix::operator==)
      .def("__ne__", &Matrix::operator!=)
      .def("__getitem__", [](const Matrix &M, std::pair<size_t,size_t> idx){
          return M(idx.first, idx.second);
      })
      .def("__setitem__", [](Matrix &M, std::pair<size_t,size_t> idx, double v){
          M(idx.first, idx.second) = v;
      })
      .def_property_readonly("nrow", &Matrix::nrow)
      .def_property_readonly("ncol", &Matrix::ncol);

    m.def("bytes",      [](){ return CustomAllocator<double>::allocated_bytes - CustomAllocator<double>::deallocated_bytes; });
    m.def("allocated",  [](){ return CustomAllocator<double>::allocated_bytes; });
    m.def("deallocated",[](){ return CustomAllocator<double>::deallocated_bytes; });
}
