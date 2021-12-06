#include "ai.h"

namespace snake {

Ai::Ai(Snake &snake, const ObstacleCanvas &canvas)
    : _snake{snake}
    , _obstacleCanvas{canvas} {}

Point Ai::update() {
    _searchCanvas.clear();

    auto head = _obstacleCanvas.snakeHeadPos;

    _searchCanvas.explore(head + head.Up(), head);
    _searchCanvas.explore(head + head.Down(), head);
    _searchCanvas.explore(head + head.Left(), head);
    _searchCanvas.explore(head + head.Right(), head);

    return {0, 0};
}

void Ai::draw(sdl::RendererView renderer) {
    const auto cellSize = _obstacleCanvas.cellSize;
    auto offset = cellSize / 2;

    renderer.drawColor({100, 100, 100, 255});

    for (int y = 0; y < _searchCanvas.height; ++y) {
        for (int x = 0; x < _searchCanvas.width; ++x) {
            auto cell = _searchCanvas.get(x, y);
            if (cell.explored) {
                auto parent = cell.parent;
                renderer.drawLine(x * cellSize + offset,
                                  y * cellSize + offset,
                                  parent.x * cellSize + offset,
                                  parent.y * cellSize + offset);
            }
        }
    }
}

} // namespace snake
