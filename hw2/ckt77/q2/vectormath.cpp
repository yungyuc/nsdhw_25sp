#include <pybind11/pybind11.h>
#include <cmath>
#include <stdexcept>

namespace py = pybind11;

double calculate_angle(double x1, double y1, double x2, double y2) {
    // 檢查零向量，直接拋出 Python ValueError
    if (x1 == 0 && y1 == 0) {
        throw py::value_error("First vector cannot be zero vector");
    }
    if (x2 == 0 && y2 == 0) {
        throw py::value_error("Second vector cannot be zero vector");
    }

    // 計算點積
    double dot_product = x1 * x2 + y1 * y2;
    
    // 計算向量長度
    double len1 = std::sqrt(x1 * x1 + y1 * y1);
    double len2 = std::sqrt(x2 * x2 + y2 * y2);
    
    // 計算夾角
    double cos_angle = dot_product / (len1 * len2);
    
    // 處理數值誤差
    if (cos_angle > 1.0) cos_angle = 1.0;
    if (cos_angle < -1.0) cos_angle = -1.0;
    
    return std::acos(cos_angle);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "A module for calculating angle between 2D vectors";
    
    m.def("calculate_angle", &calculate_angle, 
          "Calculate angle between two 2D vectors (in radians).\n\n"
          "Args:\n"
          "    x1 (float): x-coordinate of first vector\n"
          "    y1 (float): y-coordinate of first vector\n"
          "    x2 (float): x-coordinate of second vector\n"
          "    y2 (float): y-coordinate of second vector\n\n"
          "Returns:\n"
          "    float: angle in radians between the two vectors\n\n"
          "Raises:\n"
          "    ValueError: if either vector is a zero vector",
          py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"));
}
