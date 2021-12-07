#pragma once

#include "canvas.h"
#include "sdlpp/render.hpp"
#include "sdlpp/window.hpp"

namespace snake {

struct ObstacleCanvas;
struct Ai;
struct Settings;
struct SearchCanvas;
struct Snake;

struct SdlRenderer {
    SdlRenderer(const Settings &settings);

    void color(int r, int g, int b);

    void beginDraw();
    void finishDraw();

    void draw(const ObstacleCanvas &);
    void draw(const Snake &);
    void draw(const Ai &);
    void draw(const SearchCanvas &, int offset = 0);

    sdl::Window window;
    sdl::Renderer renderer;

    const Settings &settings;

    Canvas<bool> loopPrevention;
    const int cellSize = 10;
};

} // namespace snake
