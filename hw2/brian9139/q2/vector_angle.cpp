#include "vector_angle.h"
#include <cmath>
#include <stdexcept>

double vector_angle(double x1, double y1, double x2, double y2) {
    double dot_product = x1 * x2 + y1 * y2;
    double mag_v1 = std::sqrt(x1 * x1 + y1 * y1);
    double mag_v2 = std::sqrt(x2 * x2 + y2 * y2);

    if (mag_v1 == 0.0 || mag_v2 == 0.0) {
        throw std::invalid_argument("Vector magnitude cannot be zero.");
    }

    double cos_theta = dot_product / (mag_v1 * mag_v2);
    return std::acos(std::max(-1.0, std::min(1.0, cos_theta)));  // 確保 cosθ 在 [-1,1]
}
