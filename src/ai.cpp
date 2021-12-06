#include "ai.h"

namespace snake {

Ai::Ai(Snake &snake, ObstacleCanvas &canvas)
    : snake{snake}
    , canvas{canvas} {}

Point Ai::update() {
    return {0, 0};
}

} // namespace snake
