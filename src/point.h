#pragma once

#include <cstdint>

template <typename Type>
struct BasicPoint {
    Type x = 0;
    Type y = 0;
    using TypeT = Type;

    BasicPoint() = default;

    BasicPoint(const BasicPoint &other)
        : x{other.x}
        , y{other.y} {}

    BasicPoint &operator=(const BasicPoint &other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    BasicPoint(Type x, Type y)
        : x{x}
        , y{y} {}

    template <typename Other>
    BasicPoint(Other other)
        : x{static_cast<Type>(other.x)}
        , y{static_cast<Type>(other.y)} {}

    template <typename Other>
    constexpr void operator+=(Other other) {
        x += other.x;
        y += other.y;
    }

    template <typename Other>
    constexpr BasicPoint operator+(Other other) {
        return {
            static_cast<BasicPoint::TypeT>(x + other.x),
            static_cast<BasicPoint::TypeT>(y + other.y),
        };
    }

    template <typename Other>
    constexpr BasicPoint operator-(Other other) {
        return {
            static_cast<BasicPoint::TypeT>(x - other.x),
            static_cast<BasicPoint::TypeT>(y - other.y),
        };
    }

    template <typename Other>
    constexpr BasicPoint &operator=(Other other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    template <typename Other>
    constexpr bool operator=(Other other) const {
        return x == other.x && y == other.y;
    }

    static constexpr BasicPoint Up() {
        return {0, -1};
    }

    static constexpr BasicPoint Down() {
        return {0, 1};
    }

    static constexpr BasicPoint Left() {
        return {-1, 0};
    }

    static constexpr BasicPoint Right() {
        return {1, 0};
    }
};

using Point = BasicPoint<int8_t>;
using SmallPoint = BasicPoint<int8_t>;
// using Point = BasicPoint<int>;
// using SmallPoint = BasicPoint<int8_t>;
