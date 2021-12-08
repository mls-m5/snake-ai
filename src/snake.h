#pragma once

#include "gamestate.h"
#include "obstaclecanvas.h"
#include <functional>
#include <list>

namespace snake {

struct Snake {
    Snake(ObstacleCanvas &canvas, int startLen = 10)
        : _canvas{canvas}
        , _len{startLen} {
        auto start = Point{
            static_cast<Point::TypeT>(canvas.width / 2),
            static_cast<Point::TypeT>(canvas.height / 2),
        };
        auto len = std::min(_len, canvas.height / 2 - 1);
        for (int i = 0; i < len; ++i) {
            auto p =
                Point{start.x, static_cast<Point::TypeT>(start.y + len - i)};
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
            _segments.erase(_segments.begin());
        }
    }

    void putOnCanvas() {
        for (auto s : _segments) {
            _canvas.set(s, ObstacleCanvas::Snake);
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

    const std::list<Point> &segments() const {
        return _segments;
    }

    // Reset state
    void reset(const GameState &state) {
        _segments.assign(state.snakeSegments.begin(),
                         state.snakeSegments.end());
        _len = state.snakeLen;
        _direction = state.snakeDir;
        _canvas.clear();
        putOnCanvas();
        _canvas.set(state.applePos, ObstacleCanvas::Apple);
        _canvas.applePos = state.applePos;
        _isDead = state.isDead;
    }

    GameState save(int stepId) {
        return GameState{
            .snakeSegments =
                std::vector<Point>(_segments.begin(), _segments.end()),
            .snakeLen = _len,
            .snakeDir = _direction,
            .applePos = _canvas.applePos,
            .isDead = _isDead,
            .step = stepId,
        };
    }

    void logCallback(std::function<void()> f) {
        _logCallback = f;
    }

    int len() const {
        return _len;
    }

    Point tail() const {
        return _segments.front();
    }

    Point head() const {
        return _segments.back();
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
