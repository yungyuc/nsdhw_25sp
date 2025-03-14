#include <pybind11/pybind11.h>
#include <cmath>
#include <stdexcept>  // For exception handling

// Function to calculate angle between two 2D vectors (in radians)
double angleBetweenVectors(double x1, double y1, double x2, double y2) {
    // Calculate dot product
    double dot = x1 * x2 + y1 * y2;

    // Calculate magnitudes of both vectors
    double magA = std::sqrt(x1 * x1 + y1 * y1);
    double magB = std::sqrt(x2 * x2 + y2 * y2); 

    // Check for zero-length vectors to avoid division by zero
    if (magA == 0.0 || magB == 0.0) {
        throw std::invalid_argument("One or both vectors have zero length.");
    }

    // Compute cosine of angle
    double cosTheta = dot / (magA * magB);

    // Clamp the value to the valid range for acos() to avoid NaN due to precision errors
    if (cosTheta < -1.0) cosTheta = -1.0;
    if (cosTheta > 1.0) cosTheta = 1.0;

    // Return angle in radians
    return std::acos(cosTheta);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "a function to get angle"; // optional module docstring

    m.def("calculate_angle", &angleBetweenVectors, "a function to get angle");
}