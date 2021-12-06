#pragma once

#include "obstaclecanvas.h"
#include <functional>
#include <list>

namespace snake {

struct Snake {
    Snake(ObstacleCanvas &canvas)
        : _canvas{canvas} {
        auto start = Point{canvas.width / 2 * 0, canvas.height / 2};
        for (int i = 0; i < _len; ++i) {
            auto p = Point{start.x, start.y + _len - i};
            canvas.set(p.x, p.y, 1);
            _segments.push_back(p);
            canvas.snakeHeadPos = p;
        }
    };

    void update(Point requestedDirection) {
        if (_isDead) {
            return;
        }

        auto head = _segments.back();

        if (auto t = head + requestedDirection;
            _canvas.isInside(t) && _canvas.at(t) != ObstacleCanvas::Snake) {
            _direction = requestedDirection;
        }

        head += _direction;

        if (!_canvas.isInside(head)) {
            _isDead = true;
            return;
        }

        if (_canvas.at(head) == ObstacleCanvas::Apple) {
            ++_len;
            _canvas.putApple();

            _logCallback();
        }

        if (_canvas.at(head) == ObstacleCanvas::Snake) {
            _isDead = true;
            return;
        }

        _canvas.snakeHeadPos = head;
        _segments.push_back(head);
        _canvas.set(head, 1);

        if (_segments.size() > _len) {
            _canvas.set(_segments.front(), 0);
            _segments.erase(_segments.begin(), std::next(_segments.begin()));
        }
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

    void logCallback(std::function<void()> f) {
        _logCallback = f;
    }

    int len() const {
        return _len;
    }

private:
    ObstacleCanvas &_canvas;
    std::list<Point> _segments;

    Point _direction = {0, -1};
    bool _isDead = false;
    int _len = 10;
    std::function<void()> _logCallback;
};

} // namespace snake
