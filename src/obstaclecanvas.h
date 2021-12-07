#pragma once

#include "canvas.h"
#include "point.h"
#include "sdlpp/render.hpp"
#include <array>

namespace snake {

struct ObstacleCanvas : public Canvas<int> {

    enum Content {
        None = 0,
        Snake = 1,
        Apple = 2,
    };

    using DataT = int;

    void putApple();

    Point applePos;
    Point snakeHeadPos;
};

} // namespace snake
