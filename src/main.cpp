

#include "ai.h"
#include "obstaclecanvas.h"
#include "sdlpp/events.hpp"
#include "sdlpp/render.hpp"
#include "sdlpp/surface.hpp"
#include "sdlpp/window.hpp"
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
    auto window = sdl::Window{"snake ai",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              500,
                              500,
                              SDL_WINDOW_OPENGL};

    auto renderer = sdl::Renderer{window.get(), -1, SDL_RENDERER_ACCELERATED};

    if (!renderer) {
        std::cerr << "could not create renderer\n";
        exit(1);
    }

    auto canvas = ObstacleCanvas{};
    auto snake = Snake{canvas};

    canvas.putApple();
    auto ai = Ai{snake, canvas};

    Point control;

    enum ControlModes {
        Human,
        Ai,
        DryrunAi,
    };

    auto mode = Ai;

    for (; !snake.isDead();) {
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

        renderer.drawColor({10, 0, 0, 255});
        renderer.clear();

        renderer.drawColor({0, 0, 0, 255});
        canvas.draw(renderer);
        ai.draw(renderer);

        renderer.present();

        if (mode == Ai) {
            snake.update(control);
        }

        std::this_thread::sleep_for(100ms);
    }

    std::this_thread::sleep_for(1000ms);

    return 0;
}
