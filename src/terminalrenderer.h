#pragma once

#include "canvas.h"
#include "settings.h"

namespace snake {

struct ObstacleCanvas;
struct Ai;
struct Settings;
struct SearchCanvas;

struct TerminalRenderer {
    TerminalRenderer(const Settings &settings);

    void color(int r, int g, int b);

    void beginDraw();
    void finishDraw();

    void draw(const ObstacleCanvas &);
    void draw(const Ai &);
    void draw(const SearchCanvas &, int cellSize, int offset = 0);

    const Settings &settings;

    //    Canvas<bool> loopPrevention;
};

} // namespace snake
