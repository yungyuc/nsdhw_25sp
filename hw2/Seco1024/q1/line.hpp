#ifndef _LINE_HPP
#define _LINE_HPP

#include <cstdlib> 
#include <vector>

class Line {
public:
    Line();
    Line(Line const &);
    Line(Line &&) noexcept;
    Line& operator=(Line const &);
    Line& operator=(Line &&) noexcept;
    Line(size_t size);
    ~Line();
    size_t size() const;
    float const & x(size_t it) const;
    float & x(size_t it);
    float const & y(size_t it) const;
    float & y(size_t it);

private:
    std::vector<float> _x;
    std::vector<float> _y;
};


#endif