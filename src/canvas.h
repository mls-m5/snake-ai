#pragma once

#include "point.h"
#include <array>

namespace snake {

template <typename Type>
struct Canvas {
    static constexpr int width = 32;
    static constexpr int height = 32;

    auto get(int x, int y) const {
        return _data.at(y * width + x);
    }

    auto get(Point p) const {
        return get(p.x, p.y);
    }

    void set(int x, int y, Type value) {
        _data.at(y * width + x) = value;
    }

    void set(Point p, Type value) {
        set(p.x, p.y, value);
    }

    bool isInside(Point p) {
        return p.x >= 0 && p.y >= 0 && p.x < width && p.y < height;
    }

    auto &data() const {
        return _data;
    }

    Point pointFromIndex(size_t index) {
        return {index % width, index / width};
    }

private:
    std::array<Type, width *height> _data = {};
};

} // namespace snake
