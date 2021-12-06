#pragma once

#include <cstdint>

template <typename Type>
struct BasicPoint {
    Type x = 0;
    Type y = 0;

    void operator+=(BasicPoint other) {
        x += other.x;
        y += other.y;
    }

    BasicPoint operator+(BasicPoint other) {
        return {x + other.x, y + other.y};
    }
};

using Point = BasicPoint<int>;
using SmallPoint = BasicPoint<int8_t>;
