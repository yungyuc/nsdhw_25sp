#include <pybind11/pybind11.h>
#include "calc_angle.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_vector, m) {
    m.def("calc_angle", &calc_angle, "Calculate angle between two 2D vectors",
          py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"));
}