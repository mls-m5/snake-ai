#pragma once

#include "canvas.h"
#include "point.h"
#include "sdlpp/render.hpp"
#include <array>

namespace snake {

struct ObstacleCanvas : public Canvas<int> {
    const int cellSize = 10;

    enum Content {
        None = 0,
        Snake = 1,
        Apple = 2,
    };

    using DataT = int;

    void draw(sdl::RendererView renderer);

    void putApple();

    Point applePos;
    Point snakeHeadPos;
};

} // namespace snake
