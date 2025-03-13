#include <iostream>
#include <vector>

class Line {
public:
    Line() = default;
    explicit Line(size_t size): xs(size), ys(size){}
    Line(const Line& other): xs(other.xs), ys(other.ys){}
    Line(Line&& other) noexcept: xs(std::move(other.xs)), ys(std::move(other.ys)){}
    Line& operator=(const Line& other) {
        if (this != &other) {
            xs = other.xs;
            ys = other.ys;
        }
        return *this;
    }
    Line& operator=(Line&& other) noexcept {
        if (this != &other) {
            xs = std::move(other.xs);
            ys = std::move(other.ys);
        }
        return *this;
    }
    // Accessors for x coordinate.
    float const & x(size_t it) const { return xs.at(it); }
    float       & x(size_t it)       { return xs.at(it); }

    // Accessors for y coordinate.
    float const & y(size_t it) const { return ys.at(it); }
    float       & y(size_t it)       { return ys.at(it); }

    size_t size() const { return xs.size(); }

private:
    std::vector<float> xs;
    std::vector<float> ys;
};


int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}
