#pragma once

#include <cstdlib>
#include <new>
#include <memory>
#include <atomic>

template <class T>
class MyAllocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;

    MyAllocator() = default;

    template <class U> constexpr MyAllocator(const MyAllocator<U>&) noexcept {}

    static size_t get_current() { return current_byte.load(); }
    static size_t get_allocated() { return allocated.load(); }
    static size_t get_deallocated() { return deallocated.load(); }

    T* allocate(std::size_t n)
    {
        const size_type bytes = n * sizeof(T);
        current_byte += bytes;
        allocated += bytes;
        return std::allocator<T>().allocate(n); // no need to check for null
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        const size_type bytes = n * sizeof(T);
        current_byte -= bytes;
        deallocated += bytes;

        std::allocator<T>().deallocate(p, n);
    }

    template<class U>
    struct rebind {
        using other = MyAllocator<U>;
    };

private:
    static std::atomic_size_t allocated;
    static std::atomic_size_t deallocated;
    static std::atomic_size_t current_byte;
};

// static member definitions
template <class T> std::atomic_size_t MyAllocator<T>::allocated{0};
template <class T> std::atomic_size_t MyAllocator<T>::deallocated{0};
template <class T> std::atomic_size_t MyAllocator<T>::current_byte{0};

// Comparison operators
template <typename T, typename U>
constexpr bool operator==(const MyAllocator<T>&, const MyAllocator<U>&) noexcept {
    return true;
}

template <typename T, typename U>
constexpr bool operator!=(const MyAllocator<T>&, const MyAllocator<U>&) noexcept {
    return false;
}