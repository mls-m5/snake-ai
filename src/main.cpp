

#include "ai.h"
#include "logger.h"
#include "obstaclecanvas.h"
#include "sdlpp/events.hpp"
#include "sdlrenderer.h"
#include "snake.h"
#include "terminalrenderer.h"
#include <iostream>
#include <thread>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

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
        case SDL_QUIT:
            exit(0);
            break;
        }
    }

    return control;
}

// Main is written as a struct to handle emscriptens control flow
struct Main {
    const Settings settings;

    SdlRenderer renderer = {settings};

    ObstacleCanvas canvas = {};
    Snake snake = {canvas, settings.startLen};
    Logger logger = Logger{std::cout};

    int numApples = 0;
    int steps = 0;

    Ai ai = {snake, canvas, settings};

    enum class Mode {
        Human,
        Ai,
        DryrunAi,
    };

    Mode mode = Mode::Ai;

    Main(int argc, char **argv)
        : settings{argc, argv} {

        canvas.putApple();

        snake.logCallback([&] {
            ++numApples;
            logger.logProgress({
                .steps = steps,
                .apples = numApples,
                .length = snake.len(),
                .searchTimeMs = static_cast<long>(ai.lastSearchTime().count()),
            });
        });
    }

    void loop() {
        ++steps;
        Point control;
        control = [this] {
            switch (mode) {
            case Mode::Human:
                return getControl();
                break;
            case Mode::Ai:
                getControl();
                return ai.update();
                break;
            case Mode::DryrunAi: { // Update ai but let human do the moving
                ai.update();
                return getControl();
            } break;
            }
            return Point{};
        }();

        if (mode != Mode::Ai) {
            snake.update(control);
        }

        if (!settings.hideGui) {
            renderer.beginDraw();
            renderer.draw(canvas);
            renderer.draw(ai);
            renderer.draw(snake);
            renderer.finishDraw();
        }

        if (mode == Mode::Ai) {
            snake.update(control);
        }

        if (settings.msDelay) {
            std::this_thread::sleep_for(1ms * settings.msDelay);
        }
    }

    ~Main() {

        auto terminalRenderer = TerminalRenderer{settings};
        terminalRenderer.beginDraw();
        terminalRenderer.draw(canvas);
        terminalRenderer.finishDraw();

        if (!settings.hideGui && settings.msDelay) {
            for (int i = 0; i < 1000; ++i) {
                getControl();
                std::this_thread::sleep_for(100ms);
            }
        }
    }
};

#ifndef __EMSCRIPTEN__

int main(int argc, char **argv) {
    auto main = Main{argc, argv};

    for (; !main.snake.isDead();) {
        main.loop();
    }

    return 0;
}

#else

auto emMain = Main{1, 0};

void emLoop() {
    emMain.loop();

    if (emMain.snake.isDead()) {
        emscripten_cancel_main_loop();
    }
}

int main(int argc, char **argv) {
    //    emscripten_request_animation_frame_loop(emLoop, 0);
    emscripten_set_main_loop(emLoop, 60, 0);

    return 0;
}

#endif
