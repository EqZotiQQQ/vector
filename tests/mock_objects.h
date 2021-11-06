#pragma once

#include <cstring>
#include <iostream>

#define print std::cout << __PRETTY_FUNCTION__ << '\n';

/*
 * should impl rule of 3
 * */
class A3
{
    char* cstring; // raw pointer used as a handle to a dynamically-allocated memory block
    A3(const char* s, std::size_t n) // to avoid counting twice
            : cstring(new char[n]) // allocate
    {
        print
        std::memcpy(cstring, s, n); // populate
    }
public:
    A3(const char* s = "")
            : A3(s, std::strlen(s) + 1)
    {
        print
    }
    ~A3() // I. destructor
    {
        print
        delete[] cstring;  // deallocate
    }
    A3(const A3& other) // II. copy constructor
            : A3(other.cstring)
    {
        print
    }
    A3& operator=(const A3& other) // III. copy assignment
    {
        print
        if (this == &other) return *this;
        std::size_t n{std::strlen(other.cstring) + 1};
        char* new_cstring = new char[n]; // allocate
        std::memcpy(new_cstring, other.cstring, n); // populate
        delete[] cstring;  // deallocate
        cstring = new_cstring;
        return *this;
    }
public:
    operator const char *() const {
        print
        return cstring;
    } // accessor
};

/*
 * should impl rule of 5
 * */
class A5 {
    char *cstring; // raw pointer used as a handle to a dynamically-allocated memory block
public:
    A5(const char *s = "")
            : cstring(nullptr) {
        if (s) {
            print
            std::size_t n = std::strlen(s) + 1;
            cstring = new char[n];      // allocate
            std::memcpy(cstring, s, n); // populate
        }
    }

    ~A5() {
        print
        delete[] cstring;  // deallocate
    }

    A5(const A5 &other) // copy constructor
            : A5(other.cstring) {
        print
    }

    A5(A5 &&other) noexcept // move constructor
            : cstring(std::exchange(other.cstring, nullptr)) {
        print
    }

    A5 &operator=(const A5 &other) // copy assignment
    {
        print
        return *this = A5(other);
    }

    A5 &operator=(A5 &&other) noexcept // move assignment
    {
        print
        std::swap(cstring, other.cstring);
        return *this;
    }
};

/*
 * shouldn't implement something
 * */
class A0
{
    std::string cppstring;
public:
    A0(const std::string& arg = "default") : cppstring(arg) {
        print
    }
};

// default implementations
class ADefaults
{
public:
    ADefaults(const ADefaults&) = default;
    ADefaults(ADefaults&&) = default;
    ADefaults& operator=(const ADefaults&) = default;
    ADefaults& operator=(ADefaults&&) = default;
    virtual ~ADefaults() = default;
};
