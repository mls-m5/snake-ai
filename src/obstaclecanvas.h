#pragma once

#include "canvas.h"
#include "point.h"
#include "sdlpp/render.hpp"
#include <array>

namespace snake {

struct ObstacleCanvas : public Canvas<int> {
    const int cellSize = 10;
    enum Content {
        Snake = 1,
        Apple = 2,
    };

    using DataT = int;

    void draw(sdl::RendererView renderer) {
        renderer.drawColor({0, 20, 0, 255});
        renderer.fillRect({0, 0, width * cellSize, height * cellSize});

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                auto d = get(x, y);
                if (d) {
                    if (d == 1) {
                        renderer.drawColor({100, 100, 100, 255});
                    }
                    else if (d == 2) {
                        renderer.drawColor({255, 0, 0, 255});
                    }
                    renderer.fillRect(
                        {x * cellSize, y * cellSize, cellSize, cellSize});
                }
            }
        }
    }

    void putApple();
};

} // namespace snake
