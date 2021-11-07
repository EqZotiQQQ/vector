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