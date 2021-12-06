#pragma once

#include "obstaclecanvas.h"
#include "point.h"
#include "snake.h"

namespace snake {

struct Ai {
    Ai(Snake &snake, ObstacleCanvas &canvas);

    Point update();

    Snake &snake;
    ObstacleCanvas &canvas;
};

} // namespace snake
