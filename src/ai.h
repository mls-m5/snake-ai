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

    const auto &obstacleCanvas() const {
        return _obstacleCanvas;
    }

    const auto &searchCanvas() const {
        return _searchCanvas;
    }

private:
    void search(Point to, Point from, SearchCanvas &searchCanvas);
    Snake &_snake;
    const ObstacleCanvas &_obstacleCanvas;
    SearchCanvas _searchCanvas;
    SearchCanvas _returnSearchCanvas;
    std::vector<Point> _edges;
};

} // namespace snake
