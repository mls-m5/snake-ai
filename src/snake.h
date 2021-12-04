#pragma once

#include "canvas.h"
#include <list>

namespace snake {

struct Snake {
    Snake(Canvas &canvas)
        : _canvas{canvas} {
        auto start = Point{canvas.width / 2, canvas.height / 2};
        int len = 4;
        for (int i = 0; i < len; ++i) {
            auto p = Point{start.x, start.y + len - i};
            canvas.set(p.x, p.y, 1);
            _segments.push_back(p);
        }
    };

    void update(int controlls = 0) {
        auto head = _segments.back();

        head += _direction;

        _segments.push_back(head);
        _canvas.set(head, 1);

        _canvas.set(_segments.front(), 0);
        _segments.erase(_segments.begin(), std::next(_segments.begin()));
    }

    void rotate(int dir) {
        if (dir) {
            if (dir == 1) {
                _direction.x *= -1;
            }
            else {
                _direction.y *= -1;
            }
            std::swap(_direction.x, _direction.y);
        }
    }

    Canvas &_canvas;
    std::list<Point> _segments;

    Point _direction = {0, -1};
};

} // namespace snake
