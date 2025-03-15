#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include <cstddef>

class Line{
public:
    //constructor: custom constructor, copy constructor
    Line() = delete; // default constructor.
    Line(Line const & ); // copy constructor.
    Line(Line       &&) = delete; // move constructor.
    Line & operator=(Line const & ) = delete; // copy assignment operator.
    Line & operator=(Line       &&) = delete; // move assignment operator.
    
    // Custom constructor.
    Line(size_t size) : m_x(size), m_y(size), m_size(size) {}


    // Desctructor.
    ~Line() = default;

    // accessor: x,y,size
    float const & x(size_t t) const { return m_x.at(t); } // getter
    float       & x(size_t t)       { return m_x.at(t); } // setter
    float const & y(size_t t) const { return m_y.at(t); } // getter
    float       & y(size_t t)       { return m_y.at(t); } // setter
    size_t const & size()     const { return m_size; } // getter

private:
    std::vector<float> m_x;
    std::vector<float> m_y;
    size_t m_size;
};


#endif
