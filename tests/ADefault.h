#pragma once

#include <cstring>
#include <iostream>

#define print std::cout << __PRETTY_FUNCTION__ << '\n';

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
