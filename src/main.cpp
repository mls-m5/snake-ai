

#include "ai.h"
#include "logger.h"
#include "obstaclecanvas.h"
#include "sdlpp/events.hpp"
#include "sdlrenderer.h"
#include "snake.h"
#include <iostream>
#include <thread>

using namespace snake;

using namespace std::chrono_literals;

Point getControl() {
    Point control = {};

    for (auto event = sdl::pollEvent(); event; event = sdl::pollEvent()) {
        auto keyevent = event->key;
        switch (event->type) {
        case SDL_KEYDOWN:
            switch (keyevent.keysym.scancode) {
            case SDL_SCANCODE_LEFT:
                control = {-1, 0};
                break;
            case SDL_SCANCODE_RIGHT:
                control = {1, 0};
                break;
            case SDL_SCANCODE_UP:
                control = {0, -1};
                break;
            case SDL_SCANCODE_DOWN:
                control = {0, 1};
                break;
            default:
                break;
            }

            break;
        case SDL_KEYUP:
            break;
        }
    }

    return control;
}

int main(int argc, char **argv) {
    const auto settings = Settings{argc, argv};

    auto renderer = SdlRenderer{settings};

    auto canvas = ObstacleCanvas{};
    auto snake = Snake{canvas};
    auto logger = Logger{std::cout};

    auto numApples = int{0};
    auto steps = int{0};

    canvas.putApple();
    auto ai = Ai{snake, canvas, settings};

    snake.logCallback([&] {
        ++numApples;
        logger.logProgress({
            .steps = steps,
            .apples = numApples,
            .length = snake.len(),
            .searchTimeMs = ai.lastSearchTime().count(),
        });
    });

    Point control;

    enum ControlModes {
        Human,
        Ai,
        DryrunAi,
    };

    auto mode = Ai;

    for (; !snake.isDead(); ++steps) {
        control = [&ai, mode] {
            switch (mode) {
            case Human:
                return getControl();
                break;
            case Ai:
                return ai.update();
                break;
            case DryrunAi: { // Update ai but let human do the moving
                ai.update();
                return getControl();
            } break;
            }
            return Point{};
        }();

        if (mode != Ai) {
            snake.update(control);
        }

        if (!settings.hideGui) {
            renderer.draw(canvas);
            renderer.draw(ai);
            renderer.finishDraw();
        }

        if (mode == Ai) {
            snake.update(control);
        }

        if (settings.msDelay) {
            std::this_thread::sleep_for(1ms * settings.msDelay);
        }
    }

    if (settings.msDelay) {
        std::this_thread::sleep_for(1000ms);
    }

    return 0;
}
