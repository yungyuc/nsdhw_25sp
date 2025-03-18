#include <math.h>
#include <utility> 
#include <pybind11/pybind11.h>

namespace py = pybind11;

double vector_angle(std::pair<double, double> a, std::pair<double, double> b) {
    double dot_product = a.first * b.first + a.second * b.second;
    double magnitude_a = std::sqrt(a.first * a.first + a.second * a.second);
    double magnitude_b = std::sqrt(b.first * b.first + b.second * b.second);

    if (magnitude_a == 0.0f || magnitude_b == 0.0f) {
        return -1;
    }

    double cos_theta = dot_product / (magnitude_a * magnitude_b);
    if (cos_theta < -1.0f || cos_theta > 1.0f) {
        return -1;
    }

    return std::acos(cos_theta);
}

PYBIND11_MODULE(_vector, m) {
    m.def("vector_angle", &vector_angle, "Calculate angle (in radians) between two 2D vectors");
}