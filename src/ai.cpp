#include "ai.h"

namespace snake {

Ai::Ai(Snake &snake, const ObstacleCanvas &canvas, const Settings &settings)
    : _snake{snake}
    , _obstacleCanvas{canvas} {
    _edges.reserve(_obstacleCanvas.data().size());
}

Point Ai::update() {
    auto start = std::chrono::high_resolution_clock::now();

    _tailDelay.push_back(_snake.tail());
    while (_tailDelay.size() > 6) {
        _tailDelay.erase(_tailDelay.begin());
    }

    bool bailout = false;

    auto head = _snake.head();
    auto tail = _tailDelay.front();

    Point control = {0, 0};

    auto backtrack = [&](Point pos) {
        for (SearchCanvasCell cell; cell = _searchCanvas.at(pos), cell.explored;
             pos = cell.parent) {
            if (cell.parent.x == head.x && cell.parent.y == head.y) {
                _lastSearchTime =
                    std::chrono::high_resolution_clock::now() - start;
                control = pos - head;
                return;
            }
        }
        std::cout << "backtracking failed" << std::endl;
    };

    auto backtrackBlock =
        [&head](Point pos, SearchCanvas &from, SearchCanvas &to) {
            for (SearchCanvasCell cell; cell = from.at(pos), cell.explored;
                 pos = cell.parent) {
                to.set(pos, cell);
                if (cell.parent.x == head.x && cell.parent.y == head.y) {
                    break;
                }
            }
        };

    auto blockTailSearch =
        // Make sure no searching is done directly behind the tail
        [this](SearchCanvas &canvas) {
            if (_tailDelay.empty()) {
                return;
            }
            auto end = std::prev(_tailDelay.end());
            for (auto it = _tailDelay.begin(); it != end; ++it) {
                auto t = *it;
                canvas.set(t, {true});
            }
            //            for (auto t : _tailDelay) {
            //                canvas.set(t, {true});
            //            }
        };

    _searchCanvas.clear();
    blockTailSearch(_searchCanvas);

    if (search(_obstacleCanvas.applePos, head, _searchCanvas, true)) {
        _returnSearchCanvas.clear();

        blockTailSearch(_returnSearchCanvas);
        backtrackBlock(
            _obstacleCanvas.applePos, _searchCanvas, _returnSearchCanvas);

        if (search(tail,
                   //        if (search(_snake.tail(),
                   _obstacleCanvas.applePos,
                   _returnSearchCanvas,
                   true)) {
            backtrack(_obstacleCanvas.applePos);
            return control;
        }
    }

    _searchCanvas.clear();
    blockTailSearch(_searchCanvas);
    // Try again but start searching horizontally
    if (search(_obstacleCanvas.applePos, head, _searchCanvas, false)) {
        _returnSearchCanvas.clear();

        blockTailSearch(_returnSearchCanvas);
        backtrackBlock(
            _obstacleCanvas.applePos, _searchCanvas, _returnSearchCanvas);

        if (search(tail,
                   //        if (search(_snake.tail(),
                   _obstacleCanvas.applePos,
                   _returnSearchCanvas,
                   true)) {
            backtrack(_obstacleCanvas.applePos);
            return control;
        }
    }

    // Just bail out and try to follow tail
    _searchCanvas.clear();
    _returnSearchCanvas.clear();

    blockTailSearch(_searchCanvas);

    if (search(tail, _snake.head(), _searchCanvas, true)) {
        backtrack(tail);
        return control;
    }

    _searchCanvas.clear();
    // Backup
    if (search(_snake.tail(), _snake.head(), _searchCanvas, true)) {
        backtrack(_snake.tail());
        return control;
    }

    return {0, 0};
}

bool Ai::search(Point to,
                Point from,
                SearchCanvas &searchCanvas,
                bool verticalFirst) {
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

        if (verticalFirst) {
            res |= e(p + Point::Up(), p);
            res |= e(p + Point::Down(), p);
            res |= e(p + Point::Left(), p);
            res |= e(p + Point::Right(), p);
        }
        else {
            res |= e(p + Point::Left(), p);
            res |= e(p + Point::Right(), p);
            res |= e(p + Point::Up(), p);
            res |= e(p + Point::Down(), p);
        }

        if (res) {
            return true;
        }
    }
    return false;
}

} // namespace snake
