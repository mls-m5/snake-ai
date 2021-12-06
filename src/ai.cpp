#include "ai.h"

namespace snake {

Ai::Ai(Snake &snake, const ObstacleCanvas &canvas, const Settings &settings)
    : _snake{snake}
    , _obstacleCanvas{canvas} {
    _edges.reserve(_obstacleCanvas.data().size());
}

Point Ai::update() {
    auto start = std::chrono::high_resolution_clock::now();
    _searchCanvas.clear();

    bool bailout = false;

    auto head = _obstacleCanvas.snakeHeadPos;

    Point control = {0, 0};

    auto backtrack = [&](Point pos) {
        for (SearchCanvasCell cell; cell = _searchCanvas.at(pos), cell.explored;
             pos = cell.parent) {
            _returnSearchCanvas.set(pos, cell);
            if (cell.parent.x == head.x && cell.parent.y == head.y) {
                _lastSearchTime =
                    std::chrono::high_resolution_clock::now() - start;
                control = pos - head;
                break;
            }
        }
    };

    if (search(_obstacleCanvas.applePos, head, _searchCanvas)) {
        _returnSearchCanvas.clear();

        if (search(_snake.segments().front(),
                   _obstacleCanvas.applePos,
                   _returnSearchCanvas)) {
            backtrack(_obstacleCanvas.applePos);
            return control;
        }
    }

    // Could not find a way to safely eat apple
    _searchCanvas.clear();
    _returnSearchCanvas.clear();
    search(_snake.segments().back(), _snake.segments().front(), _searchCanvas);
    backtrack(_snake.segments().front());

    return control;
}

bool Ai::search(Point to, Point from, SearchCanvas &searchCanvas) {
    auto e = [&, target = to](Point to, Point from) {
        auto res = searchCanvas.exploreCell(to, from, target, _obstacleCanvas);
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
            return true;
        }
    }
    return false;
}

} // namespace snake
