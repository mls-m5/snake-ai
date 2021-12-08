#pragma once
#include "gamestate.h"
#include <list>

namespace snake {

struct Undo {
    const size_t maxLen = 20;

    void save(GameState state) {
        _states.push_back(std::move(state));

        if (_states.size() > maxLen) {
            _states.erase(_states.begin());
        }
    }

    void pop() {
        if (_states.size() > 1) {
            _states.pop_back();
        }
    }

    const GameState &state() const {
        return _states.back();
    }

    std::list<GameState> _states;
};

} // namespace snake
