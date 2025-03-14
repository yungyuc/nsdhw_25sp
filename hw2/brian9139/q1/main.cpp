#include <iostream>
#include <vector>

class Line {
public:
    Line() = default;
    Line(size_t size) : x_vals(size), y_vals(size) {}
    
    Line(const Line& other) = default;
    Line(Line&& other) noexcept = default;
    
    Line& operator=(const Line& other) = default;
    Line& operator=(Line&& other) noexcept = default;
    
    ~Line() = default;
    
    size_t size() const { return x_vals.size(); }
    
    float const& x(size_t it) const { return x_vals[it]; }
    float& x(size_t it) { return x_vals[it]; }
    
    float const& y(size_t it) const { return y_vals[it]; }
    float& y(size_t it) { return y_vals[it]; }

private:
    std::vector<float> x_vals;
    std::vector<float> y_vals;
};

int main()
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it = 0; it < line.size(); ++it) {
        std::cout << "point " << it << ": x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line2.size() << std::endl;
    for (size_t it = 0; it < line2.size(); ++it) {
        std::cout << "point " << it << ": x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }
    return 0;
}
