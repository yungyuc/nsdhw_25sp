#include "line.hpp"
#include <stdexcept>

Line::Line(size_t size) : x_coords(size), y_coords(size) {}

Line::Line(const Line& other) : x_coords(other.x_coords), y_coords(other.y_coords) {}

Line& Line::operator=(const Line& other) {
    if (this != &other) {
        x_coords = other.x_coords;
        y_coords = other.y_coords;
    }
    return *this;
}

Line::~Line() {}

size_t Line::size() const {
    return x_coords.size();
}

float const & Line::x(size_t it) const {
    return x_coords.at(it);
}

float & Line::x(size_t it) {
    return x_coords.at(it);
}

float const & Line::y(size_t it) const {
    return y_coords.at(it);
}

float & Line::y(size_t it) {
    return y_coords.at(it);
}