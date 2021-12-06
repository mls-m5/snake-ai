#pragma once

#include "obstaclecanvas.h"
#include "point.h"
#include "searchcanvas.h"
#include "settings.h"
#include "snake.h"
#include <chrono>
#include <vector>

namespace snake {

struct Ai {
    Ai(Snake &snake, const ObstacleCanvas &canvas, const Settings &settings);

    Point update();

    const auto &obstacleCanvas() const {
        return _obstacleCanvas;
    }

    const auto &searchCanvas() const {
        return _searchCanvas;
    }

    const auto &returnSearchCanvas() const {
        return _returnSearchCanvas;
    }

    auto lastSearchTime() const {
        return _lastSearchTime;
    }

    auto &snake() const {
        return _snake;
    }

private:
    bool search(Point to, Point from, SearchCanvas &searchCanvas);

    Snake &_snake;
    const ObstacleCanvas &_obstacleCanvas;
    SearchCanvas _searchCanvas;
    SearchCanvas _returnSearchCanvas;
    std::vector<Point> _edges;
    std::chrono::high_resolution_clock::duration _lastSearchTime = {};
};

} // namespace snake
