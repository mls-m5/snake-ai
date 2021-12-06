#pragma once

#include "obstaclecanvas.h"
#include "point.h"
#include "snake.h"

namespace snake {

struct Ai {
    Ai(Snake &snake, const ObstacleCanvas &canvas);

    Point update();

    Snake &snake;
    const ObstacleCanvas &canvas;
};

} // namespace snake
