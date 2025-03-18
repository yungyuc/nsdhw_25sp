#include "line.hpp"

Line::Line() {}

Line::Line(Line const &other) : _x(other._x), _y(other._y) {}

Line::Line(Line &&other) noexcept 
    : _x(std::move(other._x)), _y(std::move(other._y)) {}

Line& Line::operator=(Line const &other) {
    if (this != &other) {
        _x = other._x;
        _y = other._y;
    }
    return *this;
}

Line& Line::operator=(Line &&other) noexcept {
    if (this != &other) {
        _x = std::move(other._x);
        _y = std::move(other._y);
    }
    return *this;
}

Line::Line(size_t size) {
    _x.resize(size);
    _y.resize(size);
}

Line::~Line() {
    _x.clear();
    _y.clear();
}

size_t Line::size() const {
    return _x.size();
}

float const & Line::x(size_t it) const {
    return _x[it];
}

float & Line::x(size_t it) {
    return _x[it];
}

float const & Line::y(size_t it) const {
    return _y[it];
}

float & Line::y(size_t it) {
    return _y[it];
}