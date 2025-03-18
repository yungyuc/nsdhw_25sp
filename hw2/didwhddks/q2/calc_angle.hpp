#ifndef CALC_ANGLE_HPP
#define CALC_ANGLE_HPP

#include <cmath>
#include <stdexcept>

double calc_angle(const double &x1, const double &y1, const double &x2, const double &y2) {
    double product = x1 * x2 + y1 * y2;
    double magnitude_v1 = std::sqrt(x1 * x1 + y1 * y1);
    double magnitude_v2 = std::sqrt(x2 * x2 + y2 * y2);

    if (magnitude_v1 == 0 || magnitude_v2 == 0) {
        throw std::invalid_argument("Vector magnitude cannot be zero");
    }

    double cos = product / (magnitude_v1 * magnitude_v2);

    cos = std::fmax(-1.0, std::fmin(1.0, cos));

    return std::acos(cos);
}

#endif