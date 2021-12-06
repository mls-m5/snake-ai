#pragma once

#include "obstaclecanvas.h"
#include "point.h"
#include "searchcanvas.h"
#include "snake.h"

namespace snake {

struct Ai {
    Ai(Snake &snake, const ObstacleCanvas &canvas);

    Point update();

    void draw(sdl::RendererView renderer);

private:
    Snake &_snake;
    const ObstacleCanvas &_obstacleCanvas;
    SearchCanvas _searchCanvas;
};

} // namespace snake
