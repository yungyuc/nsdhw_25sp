#ifndef LINE_HPP
#define LINE_HPP

#include <cstddef>
#include <vector>

class Line {
public:
    Line() {
        X.assign(1, 0);
        Y.assign(1, 0);
    }
    Line(Line const &line) {
        X = line.X;
        Y = line.Y;
    }
    Line(Line &&line) {
        X = std::move(line.X);
        Y = std::move(line.Y);
    }
    Line & operator=(Line const &line) {
        X = line.X;
        Y = line.Y;
        return *this;
    }
    Line & operator=(Line &&line) {
        X = std::move(line.X);
        Y = std::move(line.Y);
        return *this;
    }
    Line(size_t size) {
        X.assign(size, 0);
        Y.assign(size, 0);
    }
    ~Line() {}
    size_t size() const {
        return X.size();
    }
    float const & x(size_t it) const {
        return X[it];
    }
    float & x(size_t it) {
        return X[it];
    }
    float const & y(size_t it) const {
        return Y[it];
    }
    float & y(size_t it) {
        return Y[it];
    }
private:
    std::vector<float> X;
    std::vector<float> Y;
};

#endif