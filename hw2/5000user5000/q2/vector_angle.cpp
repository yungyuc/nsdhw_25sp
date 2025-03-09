#include "vector_angle.hpp"
#include <algorithm>  // for std::clamp
#include <cmath>
#include <stdexcept>

double calculate_angle(double x1, double y1, double x2, double y2) {
    double dot = x1 * x2 + y1 * y2;
    double mag1 = std::sqrt(x1 * x1 + y1 * y1);
    double mag2 = std::sqrt(x2 * x2 + y2 * y2);

    if (mag1 == 0.0 || mag2 == 0.0) {
        throw std::invalid_argument("Zero-length vector provided");
    }

    double cosine = dot / (mag1 * mag2);
    cosine = std::clamp(cosine, -1.0, 1.0);  // 確保值在 acos() 的定義域內

    //  額外處理浮點數誤差
    if (std::abs(cosine - 1.0) < 1e-9) return 0.0;  // 避免 acos(0.9999999999999)
    if (std::abs(cosine + 1.0) < 1e-9) return M_PI; // 避免 acos(-0.9999999999999)

    return std::acos(cosine);
}
