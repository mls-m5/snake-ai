#pragma once

#include "obstaclecanvas.h"
#include "point.h"
#include "searchcanvas.h"
#include "snake.h"
#include <vector>

namespace snake {

struct Ai {
    Ai(Snake &snake, const ObstacleCanvas &canvas);

    Point update();

    void draw(sdl::RendererView renderer);

private:
    void explore(Point from);
    Snake &_snake;
    const ObstacleCanvas &_obstacleCanvas;
    SearchCanvas _searchCanvas;
    std::vector<Point> _edges;
};

} // namespace snake
