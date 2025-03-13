#include <pybind11/pybind11.h>
#include "vector_angle.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_vector, m) {  // 確保名稱是 `_vector`，與 Python import 一致
    m.def("calculate_angle", &calculate_angle, "Calculate angle between two 2D vectors");
}
