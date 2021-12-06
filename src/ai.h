#pragma once

#include "obstaclecanvas.h"
#include "point.h"
#include "searchcanvas.h"
#include "settings.h"
#include "snake.h"
#include <vector>

namespace snake {

struct Ai {
    Ai(Snake &snake, const ObstacleCanvas &canvas, const Settings &settings);

    Point update();

    void draw(sdl::RendererView renderer);

private:
    void explore(Point from);
    Snake &_snake;
    const ObstacleCanvas &_obstacleCanvas;
    const Settings &_settings;
    SearchCanvas _searchCanvas;
    std::vector<Point> _edges;
};

} // namespace snake
