#include "line.hpp"

Line::Line() {}

Line::Line(size_t size) : x_coords(size), y_coords(size) {}

Line::Line(const Line &other) : x_coords(other.x_coords), y_coords(other.y_coords) {}

Line::Line(Line &&other) noexcept : x_coords(std::move(other.x_coords)), y_coords(std::move(other.y_coords)) {}

Line& Line::operator=(const Line &other) {
    if (this != &other) {
        x_coords = other.x_coords;
        y_coords = other.y_coords;
    }
    return *this;
}

Line& Line::operator=(Line &&other) noexcept {
    if (this != &other) {
        x_coords = std::move(other.x_coords);
        y_coords = std::move(other.y_coords);
    }
    return *this;
}

Line::~Line() {}

size_t Line::size() const { return x_coords.size(); }

float const &Line::x(size_t index) const { return x_coords.at(index); }
float &Line::x(size_t index) { return x_coords.at(index); }
float const &Line::y(size_t index) const { return y_coords.at(index); }
float &Line::y(size_t index) { return y_coords.at(index); }