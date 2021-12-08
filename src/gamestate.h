#pragma once

#include "point.h"
#include <vector>

namespace snake {

// State used for saving and loading, and undoing
struct GameState {
    std::vector<Point> snakeSegments;
    int snakeLen;
    Point snakeDir;
    Point applePos;
    bool isDead;
    int step;

    std::vector<Point> aiTailDelay;
};

} // namespace snake
