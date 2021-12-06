#include "ai.h"

namespace snake {

Ai::Ai(Snake &snake, const ObstacleCanvas &canvas)
    : _snake{snake}
    , _obstacleCanvas{canvas} {
    _edges.reserve(_obstacleCanvas.data().size());
}

Point Ai::update() {
    _searchCanvas.clear();

    auto head = _obstacleCanvas.snakeHeadPos;

    explore(head);

    Point pos = _obstacleCanvas.applePos;
    for (SearchCanvasCell cell; cell = _searchCanvas.at(pos), cell.explored;
         pos = cell.parent) {
        if (cell.parent.x == head.x && cell.parent.y == head.y) {
            return pos - head;
        }
    }
    return {0, 0};
}

void Ai::draw(sdl::RendererView renderer) {
    const auto cellSize = _obstacleCanvas.cellSize;
    auto offset = cellSize / 2;

    renderer.drawColor({100, 100, 100, 255});

    auto drawLine = [&](Point from, Point to) {
        renderer.drawLine(from.x * cellSize + offset,
                          from.y * cellSize + offset,
                          to.x * cellSize + offset,
                          to.y * cellSize + offset);
    };

    for (int y = 0; y < _searchCanvas.height; ++y) {
        for (int x = 0; x < _searchCanvas.width; ++x) {
            auto cell = _searchCanvas.at(x, y);
            if (cell.explored) {
                auto parent = cell.parent;
                renderer.drawLine(x * cellSize + offset,
                                  y * cellSize + offset,
                                  parent.x * cellSize + offset,
                                  parent.y * cellSize + offset);
            }
        }
    }

    // Draw the real path to apple
    renderer.drawColor({255, 255, 255, 255});
    Point pos = _obstacleCanvas.applePos;
    for (SearchCanvasCell cell; cell = _searchCanvas.at(pos), cell.explored;
         pos = cell.parent) {
        drawLine(pos, cell.parent);
    }
}

void Ai::explore(Point from) {
    auto e = [&](Point to, Point from) {
        auto res = _searchCanvas.exploreCell(to, from, _obstacleCanvas);
        if (res == SearchCanvas::Explored) {
            _edges.push_back(to);
        }

        return res == SearchCanvas::Apple;
    };

    _edges.clear();
    _edges.push_back(from);
    //    _searchCanvas.set(from, {true, {}});

    for (int current = 0; current < _edges.size(); ++current) {
        auto p = _edges.at(current);

        bool res = 0;

        res |= e(p + Point::Up(), p);
        res |= e(p + Point::Down(), p);
        res |= e(p + Point::Left(), p);
        res |= e(p + Point::Right(), p);

        if (res) {
            break;
        }
    }
}

} // namespace snake
