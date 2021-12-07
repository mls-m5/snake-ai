#pragma once

#include "point.h"
#include <array>

namespace snake {

template <typename Type>
struct Canvas {
    static constexpr int width = 32;
    static constexpr int height = 32;

    auto at(int x, int y) const {
        return _data.at(y * width + x);
    }

    auto at(Point p) const {
        return at(p.x, p.y);
    }

    void set(int x, int y, Type value) {
        _data.at(y * width + x) = value;
    }

    void set(Point p, Type value) {
        set(p.x, p.y, value);
    }

    bool isInside(Point p) const {
        return p.x >= 0 && p.y >= 0 && p.x < width && p.y < height;
    }

    auto &data() const {
        return _data;
    }

    Point pointFromIndex(size_t index) {
        return {static_cast<int>(index % width),
                static_cast<int>(index / width)};
    }

    void fill(Type value) {
        for (auto &cell : _data) {
            cell = value;
        }
    }

    void clear() {
        fill(Type{});
    }

private:
    std::array<Type, width *height> _data = {};
};

} // namespace snake
