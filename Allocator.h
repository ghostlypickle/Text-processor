#ifndef UNTITLED13_ALLOCATOR_H
#define UNTITLED13_ALLOCATOR_H
#include <iostream>
#include <stdexcept>
#include <cstddef>
#include <limits>

template <typename T>
class Allocator {
public:
    // Define type aliases
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    // Default constructor
    Allocator() = default;

    // Destructor
    ~Allocator() = default;

    // Allocate memory for n objects
    pointer allocate(size_type n)
    {
        if (n > max_size())
            throw std::bad_alloc();

        pointer p = static_cast<pointer>(::operator new(n * sizeof(T)));
        if (!p)
            throw std::bad_alloc();

        return p;
    }

    // Deallocate memory
    void deallocate(pointer p, size_type n)
    {
        ::operator delete(p);
    }

    // Construct an object at the given memory location with provided arguments
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args)
    {
        ::new(reinterpret_cast<void*>(p)) T(std::forward<Args>(args)...);
    }

    // Destroy an object at the given memory location
    template <typename U>
    void destroy(U* p)
    {
        p->~U();
    }

    // Return the maximum number of objects that can be allocated
    size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }



    // Rebind the allocator to a different type
    template <typename U>
    struct rebind {
        using other = Allocator<U>;
    };
};

#endif //UNTITLED13_ALLOCATOR_H
