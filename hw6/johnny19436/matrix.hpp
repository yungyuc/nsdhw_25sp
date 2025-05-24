#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <cassert>
#include <cmath>
#include <stdexcept>

// Custom allocator to track memory usage
template <typename T>
class MemoryTrackingAllocator {
public:
    using value_type = T;
    
    // Static counters to track memory usage
    static size_t bytes_used;
    static size_t bytes_allocated;
    static size_t bytes_deallocated;
    
    MemoryTrackingAllocator() = default;
    
    template <typename U>
    MemoryTrackingAllocator(const MemoryTrackingAllocator<U>&) noexcept {}
    
    T* allocate(size_t n) {
        size_t bytes = n * sizeof(T);
        bytes_allocated += bytes;
        bytes_used += bytes;
        return static_cast<T*>(::operator new(bytes));
    }
    
    void deallocate(T* p, size_t n) noexcept {
        size_t bytes = n * sizeof(T);
        bytes_deallocated += bytes;
        bytes_used -= bytes;
        ::operator delete(p);
    }
};

// Initialize static members
template <typename T>
size_t MemoryTrackingAllocator<T>::bytes_used = 0;

template <typename T>
size_t MemoryTrackingAllocator<T>::bytes_allocated = 0;

template <typename T>
size_t MemoryTrackingAllocator<T>::bytes_deallocated = 0;

// Equality comparison for allocators
template <typename T, typename U>
bool operator==(const MemoryTrackingAllocator<T>&, const MemoryTrackingAllocator<U>&) {
    return true;
}

template <typename T, typename U>
bool operator!=(const MemoryTrackingAllocator<T>& a, const MemoryTrackingAllocator<U>& b) {
    return !(a == b);
}

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol);

    double& operator()(size_t i, size_t j);
    const double& operator()(size_t i, size_t j) const;
    bool operator==(const Matrix& other) const;
    size_t nrow() const;
    size_t ncol() const;

private:
    size_t nrow_, ncol_;
    std::vector<double, MemoryTrackingAllocator<double>> data_;
};

// Matrix multiplication functions
Matrix multiply_naive(const Matrix &A, const Matrix &B);
Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t tile_size = 32);
Matrix multiply_mkl(const Matrix &A, const Matrix &B);

// Memory tracking functions
size_t bytes_used();
size_t bytes_allocated();
size_t bytes_deallocated();

#endif // MATRIX_HPP
