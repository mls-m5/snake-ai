#pragma once

#include "point.h"
#include "sdlpp/render.hpp"
#include <array>

namespace snake {

struct Canvas {
    static constexpr int width = 32;
    static constexpr int height = 32;

    using DataT = int;

    void draw(sdl::RendererView renderer) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                auto d = _data.at(x + y * width);
                if (d) {
                    if (d == 1) {
                        renderer.drawColor({100, 100, 100, 255});
                    }
                    else if (d == 2) {
                        renderer.drawColor({255, 0, 0, 255});
                    }
                    renderer.fillRect(
                        {x * _cellSize, y * _cellSize, _cellSize, _cellSize});
                }
            }
        }
    }

    auto get(int x, int y) const {
        return _data.at(y * width + x);
    }

    auto get(Point p) const {
        return get(p.x, p.y);
    }

    void set(int x, int y, DataT value) {
        _data.at(y * width + x) = value;
    }

    void set(Point p, DataT value) {
        set(p.x, p.y, value);
    }

    bool isInside(Point p) {
        return p.x >= 0 && p.y >= 0 && p.x < width && p.y < height;
    }

private:
    const int _cellSize = 10;
    std::array<int, width *height> _data = {};
};

} // namespace snake
