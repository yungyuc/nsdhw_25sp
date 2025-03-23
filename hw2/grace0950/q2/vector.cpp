#include "vector.hpp"
#include <cmath>     // for sqrt and acos
#include <algorithm> // for max and min

double angle_between_vectors(double x1, double y1, double x2, double y2)
{
  // Compute dot product
  double dot = x1 * x2 + y1 * y2;
  // Compute magnitudes
  double mag1 = std::sqrt(x1 * x1 + y1 * y1);
  double mag2 = std::sqrt(x2 * x2 + y2 * y2);
  if (mag1 == 0 && mag2 == 0)
  {
    throw std::invalid_argument("Zero-length vector detected");
  }
  if (mag1 == 0 || mag2 == 0)
  {
    return 0.0;
  }
  // Clamp value between -1 and 1 to avoid domain error in acos
  double cosine = std::max(-1.0, std::min(1.0, dot / (mag1 * mag2)));
  if (std::abs(cosine - 1.0) < 1e-10)
  {
    return 0.0;
  }
  if (std::abs(cosine + 1.0) < 1e-10)
  {
    return M_PI;
  }
  return std::acos(cosine);
}
