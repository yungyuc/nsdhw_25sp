#include "_matrix.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
namespace py = pybind11;

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "A module do matrix multiplication"; // optional module docstring

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<const Matrix &>())
        .def("__eq__", [](const Matrix& a, const Matrix& b) { return operator==(a, b); })
        .def("__ne__", [](const Matrix& a, const Matrix& b) { return operator!=(a, b); })
        .def("__getitem__", [](const Matrix &mat, const std::pair<size_t, size_t> &idx)
            { return mat(idx.first, idx.second); })
        .def("__setitem__", [](Matrix &mat, const std::pair<size_t, size_t> &idx, double value)
            { mat(idx.first, idx.second) = value; })
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol);

    m.def("bytes", &MyAllocator<double>::getbytes)
    .def("allocated", &MyAllocator<double>::getallocated)
    .def("deallocated", &MyAllocator<double>::getdeallocated);


    m.def("multiply_mkl", &multiply_mkl, "do mm with mkl");
    m.def("multiply_tile", [](Matrix const & mat1, Matrix const & mat2, size_t lsize){
        return simple_tile(mat1, mat2, lsize);
    });
    m.def("multiply_naive", &multiply_naive, "do mm with naive method");
}