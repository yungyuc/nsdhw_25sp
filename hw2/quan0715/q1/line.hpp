#include <vector>
#include <cstddef>

class Line {
    public:
        Line(size_t size);
        Line(const Line& other);
        Line& operator=(const Line& other);
        ~Line();
        size_t size() const;
        float const & x(size_t it) const;
        float & x(size_t it);
        float const & y(size_t it) const;
        float & y(size_t it);
    private:
        std::vector<float> x_coords;
        std::vector<float> y_coords;
};