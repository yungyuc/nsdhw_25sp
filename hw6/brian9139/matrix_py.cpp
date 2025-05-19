#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "matrix.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m) {
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__getitem__",
             [](const Matrix &mat, std::pair<size_t, size_t> idx) {
                 return mat(idx.first, idx.second);
             })
        .def("__setitem__",
             [](Matrix &mat, std::pair<size_t, size_t> idx, double v) {
                 mat(idx.first, idx.second) = v;
             })
        .def("__eq__", &Matrix::operator==)
        // expose the internal buffer as a numpy.ndarray
        .def_property_readonly("array",
            [](Matrix &mat) {
                // shape: (rows, cols)
                std::vector<ssize_t> shape  = { (ssize_t)mat.nrow(), (ssize_t)mat.ncol() };
                // strides: row-major
                std::vector<ssize_t> strides = {
                    (ssize_t)(sizeof(double) * mat.ncol()),
                    (ssize_t)sizeof(double)
                };
                return py::array_t<double>(
                    shape,
                    strides,
                    mat.data(),
                    // keep parent alive
                    py::cast(&mat, py::return_value_policy::reference_internal)
                );
            },
            "A numpy.ndarray view of the matrix data"
        );

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
}
