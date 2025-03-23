#include "line.hpp"

// Default constructor
Line::Line() : xs(), ys() {}

// Constructor with number of points
Line::Line(size_t size) : xs(size), ys(size) {}

// Copy constructor
Line::Line(Line const& other) : xs(other.xs), ys(other.ys) {}

// Move constructor
Line::Line(Line&& other) : xs(std::move(other.xs)), ys(std::move(other.ys)) {}

// Copy assignment operator
Line& Line::operator=(Line const& other) {
    if (this != &other) {
        xs = other.xs;
        ys = other.ys;
    }
    return *this;
}

// Move assignment operator
Line& Line::operator=(Line&& other) {
    if (this != &other) {
        xs = std::move(other.xs);
        ys = std::move(other.ys);
    }
    return *this;
}

// Destructor
Line::~Line() = default;

// Return the number of points
size_t Line::size() const {
    return xs.size();
}

// Access x coordinate (const)
float const& Line::x(size_t it) const {
    return xs[it];
}

// Access x coordinate (non-const)
float& Line::x(size_t it) {
    return xs[it];
}

// Access y coordinate (const)
float const& Line::y(size_t it) const {
    return ys[it];
}

// Access y coordinate (non-const)
float& Line::y(size_t it) {
    return ys[it];
}
