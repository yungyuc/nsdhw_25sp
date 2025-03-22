#include <pybind11/pybind11.h>
#include <cmath>

namespace py = pybind11;

// 計算兩個二維向量的夾角
// 第一個向量: (x1, x2)
// 第二個向量: (x3, x4)
double angle(double x1, double x2, double x3, double x4) {
    // 計算點積
    double dot_product = x1 * x3 + x2 * x4;
    
    // 計算兩個向量的長度
    double len1 = std::sqrt(x1 * x1 + x2 * x2);
    double len2 = std::sqrt(x3 * x3 + x4 * x4);
    
    // 檢查向量長度是否為零
    if (len1 < 1e-10 || len2 < 1e-10) {
        throw std::invalid_argument("Vectors cannot have zero length");
    }
    
    // 計算角度（弧度）
    double cos_angle = dot_product / (len1 * len2);
    
    // 處理數值誤差
    if (cos_angle > 1.0) cos_angle = 1.0;
    if (cos_angle < -1.0) cos_angle = -1.0;
    
    // 返回角度（弧度）
    return std::acos(cos_angle);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "Vector angle calculation module";
    m.def("angle", &angle, "Calculate the angle between two 2D vectors (x1, x2) and (x3, x4)",
          py::arg("x1"), py::arg("x2"), py::arg("x3"), py::arg("x4"));
}