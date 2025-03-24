#pragma once
#include <vector>
#include <cstddef> // for size_t

class Line
{
public:
    Line();                                // Default constructor
    Line(size_t size);                     // Constructor with number of points
    Line(Line const& other);               // Copy constructor
    Line(Line&& other);                    // Move constructor
    Line& operator=(Line const& other);    // Copy assignment
    Line& operator=(Line&& other);         // Move assignment
    ~Line();                               // Destructor

    size_t size() const;                   // Number of points

    float const& x(size_t it) const;       // Access x (const)
    float& x(size_t it);                   // Access x (non-const)
    float const& y(size_t it) const;       // Access y (const)
    float& y(size_t it);                   // Access y (non-const)

private:
    std::vector<float> xs; // x coordinates
    std::vector<float> ys; // y coordinates
};
