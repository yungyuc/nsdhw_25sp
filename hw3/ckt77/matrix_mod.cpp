#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "matrix.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "Matrix multiplication module with naive, tiling, and MKL implementations";

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<const Matrix &>())
        .def("__eq__", &Matrix::operator==)
        .def("__ne__", &Matrix::operator!=)
        .def("__getitem__", [](const Matrix &mat, const std::pair<size_t, size_t> &idx)
             { return mat(idx.first, idx.second); })
        .def("__setitem__", [](Matrix &mat, const std::pair<size_t, size_t> &idx, double value)
             { mat(idx.first, idx.second) = value; })
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol);

    m.def("multiply_naive", &multiply_naive, "Naive matrix multiplication");
    m.def("multiply_tile", &multiply_tile, "Tiled matrix multiplication",
          py::arg("mat1"), py::arg("mat2"), py::arg("tile_size") = 16);
    m.def("multiply_mkl", &multiply_mkl, "MKL DGEMM matrix multiplication");
}
