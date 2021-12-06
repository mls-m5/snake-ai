#pragma once

#include "obstaclecanvas.h"
#include <list>

namespace snake {

struct Snake {
    Snake(ObstacleCanvas &canvas)
        : _canvas{canvas} {
        auto start = Point{canvas.width / 2, canvas.height / 2};
        int len = 10;
        for (int i = 0; i < len; ++i) {
            auto p = Point{start.x, start.y + len - i};
            canvas.set(p.x, p.y, 1);
            _segments.push_back(p);
        }
    };

    void update(Point requestedDirection) {
        if (_isDead) {
            return;
        }

        auto head = _segments.back();

        if (auto t = head + requestedDirection;
            _canvas.isInside(t) && _canvas.get(t) != ObstacleCanvas::Snake) {
            _direction = requestedDirection;
        }

        head += _direction;

        if (!_canvas.isInside(head)) {
            _isDead = true;
            return;
        }

        if (_canvas.get(head) == ObstacleCanvas::Snake) {
            _isDead = true;
            return;
        }

        _segments.push_back(head);
        _canvas.set(head, 1);

        _canvas.set(_segments.front(), 0);
        _segments.erase(_segments.begin(), std::next(_segments.begin()));
    }

    // For two finger controller
    void rotate(int dir) {
        if (dir) {
            if (dir == 1) {
                _direction.y *= -1;
            }
            else {
                _direction.x *= -1;
            }
            std::swap(_direction.x, _direction.y);
        }
    }

    bool isDead() const {
        return _isDead;
    }

    std::list<Point> segments() const {
        return _segments;
    }

private:
    ObstacleCanvas &_canvas;
    std::list<Point> _segments;

    Point _direction = {0, -1};
    bool _isDead = false;
};

} // namespace snake
