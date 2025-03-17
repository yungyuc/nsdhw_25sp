#include <pybind11/pybind11.h>
#include "vector_angle.h"

namespace py = pybind11;

PYBIND11_MODULE(_vector, m) {
    m.def("vector_angle", &vector_angle, "Calculate the angle (in radians) between two 2D vectors");
}
