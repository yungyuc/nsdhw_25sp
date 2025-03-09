#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include <iostream>

class Line {
public:
    Line();
    Line(size_t size);
    Line(const Line &other);
    Line(Line &&other) noexcept;
    Line& operator=(const Line &other);
    Line& operator=(Line &&other) noexcept;
    ~Line();

    size_t size() const;
    float const &x(size_t index) const;
    float &x(size_t index);
    float const &y(size_t index) const;
    float &y(size_t index);

private:
    std::vector<float> x_coords;
    std::vector<float> y_coords;
};

#endif // LINE_HPP