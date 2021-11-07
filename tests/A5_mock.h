#include <cstring>
#include <utility>

/*
 * should impl rule of 5
 * */
struct A5 {
    char *cstring; // raw pointer used as a handle to a dynamically-allocated memory block

    A5(const char *s = "")
            : cstring(nullptr) {
        if (s) {
            std::size_t n = std::strlen(s) + 1;
            cstring = new char[n];      // allocate
            std::memcpy(cstring, s, n); // populate
        }
    }

    ~A5() {
        delete[] cstring;  // deallocate
    }

    A5(const A5 &other) // copy constructor
            : A5(other.cstring) {
    }

    A5(A5 &&other) noexcept // move constructor
            : cstring(std::exchange(other.cstring, nullptr)) {
    }

    A5 &operator=(const A5 &other) // copy assignment
    {
        return *this = A5(other);
    }

    A5 &operator=(A5 &&other) noexcept // move assignment
    {
        std::swap(cstring, other.cstring);
        return *this;
    }
};