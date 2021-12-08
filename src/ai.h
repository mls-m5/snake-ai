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

    auto trackedTail() const {
        return _tailDelay.front();
    }

    auto &tailDelay() const {
        return _tailDelay;
    }

    void save(GameState &state) {
        state.aiTailDelay.assign(_tailDelay.begin(), _tailDelay.end());
    }

    void reset(const GameState &state) {
        _tailDelay.assign(state.aiTailDelay.begin(), state.aiTailDelay.end());
    }

private:
    bool search(Point to,
                Point from,
                SearchCanvas &searchCanvas,
                bool verticalFirst);

    Snake &_snake;
    const ObstacleCanvas &_obstacleCanvas;
    SearchCanvas _searchCanvas;
    SearchCanvas _returnSearchCanvas;
    std::vector<Point> _edges;
    std::chrono::high_resolution_clock::duration _lastSearchTime = {};

    std::list<Point> _tailDelay;
};

} // namespace snake
