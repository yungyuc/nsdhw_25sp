#include <cmath>
#include <stdexcept>
#include <pybind11/pybind11.h>

namespace py = pybind11;

double angle_between(double ax, double ay, double bx, double by) {
    double dot = ax * bx + ay * by;
    double mag_a = std::sqrt(ax * ax + ay * ay);
    double mag_b = std::sqrt(bx * bx + by * by);
    if (mag_a == 0.0 || mag_b == 0.0) {
        throw std::invalid_argument("Zero-length vector provided");
    }
    double cos_theta = dot / (mag_a * mag_b);
    // Clamp cos_theta to the interval [-1, 1] to avoid domain errors
    if (cos_theta > 1.0) { cos_theta = 1.0; }
    if (cos_theta < -1.0) { cos_theta = -1.0; }
    return std::acos(cos_theta);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "Module for computing angle between two 2D vectors";
    m.def("angle_between", &angle_between,
          "Calculate the angle (in radians) between two 2D vectors",
          py::arg("ax"), py::arg("ay"), py::arg("bx"), py::arg("by"));
}
