#include "ai.h"

namespace snake {

Ai::Ai(Snake &snake, const ObstacleCanvas &canvas, const Settings &settings)
    : _snake{snake}
    , _obstacleCanvas{canvas} {
    _edges.reserve(_obstacleCanvas.data().size());
}

Point Ai::update() {
    _searchCanvas.clear();

    auto head = _obstacleCanvas.snakeHeadPos;

    search(_obstacleCanvas.applePos, head, _searchCanvas);

    _returnSearchCanvas.clear();

    Point pos = _obstacleCanvas.applePos;
    for (SearchCanvasCell cell; cell = _searchCanvas.at(pos), cell.explored;
         pos = cell.parent) {
        _returnSearchCanvas.set(pos, cell);
        if (cell.parent.x == head.x && cell.parent.y == head.y) {
            return pos - head;
        }
    }
    return {0, 0};
}

void Ai::draw(sdl::RendererView renderer) {}

void Ai::search(Point to, Point from, SearchCanvas &searchCanvas) {
    auto e = [&](Point to, Point from) {
        auto res = searchCanvas.exploreCell(
            to, from, _obstacleCanvas.applePos, _obstacleCanvas);
        if (res == SearchCanvas::Explored) {
            _edges.push_back(to);
        }

        return res == SearchCanvas::Apple;
    };

    _edges.clear();
    _edges.push_back(from);

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
