

#include "ai.h"
#include "logger.h"
#include "obstaclecanvas.h"
#include "sdlpp/events.hpp"
#include "sdlrenderer.h"
#include "snake.h"
#include "terminalrenderer.h"
#include "undo.h"
#include <iostream>
#include <thread>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

using namespace snake;

using namespace std::chrono_literals;

struct Controls {
    Point direction;
    bool isPausePressed = false;
    bool isReversePressed = false;
    bool isSingleStepPressed = false;
};

Controls getControl() {
    Controls ret;
    for (auto event = sdl::pollEvent(); event; event = sdl::pollEvent()) {
        auto keyevent = event->key;
        switch (event->type) {
        case SDL_KEYDOWN:
            switch (keyevent.keysym.scancode) {
            case SDL_SCANCODE_LEFT:
                ret.direction = {-1, 0};
                break;
            case SDL_SCANCODE_RIGHT:
                ret.direction = {1, 0};
                break;
            case SDL_SCANCODE_UP:
                ret.direction = {0, -1};
                break;
            case SDL_SCANCODE_DOWN:
                ret.direction = {0, 1};
                break;
            case SDL_SCANCODE_SPACE:
                ret.isPausePressed = true;
                break;
            case SDL_SCANCODE_BACKSPACE:
                ret.isReversePressed = true;
                break;
            case SDL_SCANCODE_RETURN:
                ret.isSingleStepPressed = true;
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

    return ret;
}

// Main is written as a struct to handle emscriptens control flow
struct Main {
    const Settings settings;

    SdlRenderer renderer = {settings};

    ObstacleCanvas canvas = {};
    Snake snake = {canvas, settings.startLen};
    Logger logger = Logger{std::cout};
    Undo undo;

    int numApples = 0;
    int steps = 0;

    Ai ai = {snake, canvas, settings};

    enum class Mode {
        Human,
        Ai,
        DryrunAi,
    };

    Mode mode = Mode::Ai;

    bool isPaused = false;

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

        if (settings.shouldSave) {
            undo.save(snake.save(0));
        }
    }

    void render() {

        if (!settings.hideGui) {
            renderer.beginDraw();
            renderer.draw(canvas);
            renderer.draw(ai);
            renderer.draw(snake);
            renderer.finishDraw();
        }
    }

    void undoStep() {
        isPaused = true;

        undo.pop();
        snake.reset(undo.state());
        ai.reset(undo.state());
        steps = undo.state().step;

        std::cout << "resetting to step " << undo.state().step << std::endl;

        render();
    }

    void loop() {
        ++steps;
        auto control = [this] {
            auto control = getControl();
            if (!isPaused || control.isSingleStepPressed) {
                switch (mode) {
                case Mode::Human:
                    return control;
                case Mode::Ai:
                    control.direction = ai.update();
                    return control;
                case Mode::DryrunAi: { // Update ai but let human do the moving
                    ai.update();
                    return control;
                } break;
                }
            }
            return control;
        }();

        if (settings.shouldSave && control.isReversePressed) {
            undoStep();
            return;
        }

        if (control.isPausePressed) {
            isPaused = !isPaused;
        }

        auto shouldStep = !isPaused || control.isSingleStepPressed;

        if (shouldStep && mode != Mode::Ai) {
            snake.update(control.direction);
        }

        render();

        if (shouldStep && mode == Mode::Ai) {
            snake.update(control.direction);
        }

        if (!shouldStep) {
            return;
        }

        if (snake.isDead()) {
            isPaused = true;
            undoStep();
            return;
        }

        if (settings.shouldSave) {
            auto state = snake.save(steps);
            ai.save(state);
            undo.save(std::move(state));
        }
    }

    ~Main() {
        auto terminalRenderer = TerminalRenderer{settings};
        terminalRenderer.beginDraw();
        terminalRenderer.draw(canvas);
        terminalRenderer.finishDraw();
    }
};

#ifndef __EMSCRIPTEN__

int main(int argc, char **argv) {
    auto main = Main{argc, argv};

    for (; !main.snake.isDead();) {
        main.loop();
        if (main.settings.msDelay) {
            std::this_thread::sleep_for(1ms * main.settings.msDelay);
        }
        if (main.isPaused) {
            std::this_thread::sleep_for(100ms);
        }
    }

    std::cout << "died :/" << std::endl;

    if (!main.settings.hideGui && main.settings.msDelay) {
        for (int i = 0; i < 1000; ++i) {
            getControl();
            std::this_thread::sleep_for(100ms);
        }
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
    emscripten_set_main_loop(emLoop, 1000 / emMain.settings.msDelay, 0);

    return 0;
}

#endif
