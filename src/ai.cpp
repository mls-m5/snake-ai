#include "ai.h"

namespace snake {

Ai::Ai(Snake &snake, const ObstacleCanvas &canvas)
    : _snake{snake}
    , _obstacleCanvas{canvas} {}

Point Ai::update() {
    return {0, 0};
}

void Ai::draw(sdl::RendererView renderer) {
    const auto cellSize = _obstacleCanvas.cellSize;

    renderer.drawColor({255, 255, 255, 50});

    for (int y = 0; y < _searchCanvas.height; ++y) {
        for (int x = 0; x < _searchCanvas.width; ++x) {
            renderer.drawLine(x * cellSize,
                              y * cellSize,
                              (x + 1) * cellSize,
                              (y + 1) * cellSize);
        }
    }
}

} // namespace snake
