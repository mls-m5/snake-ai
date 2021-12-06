

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

    for (; !snake.isDead();) {
        switch (2) {
        case 0:
            snake.update(getControl());
            break;
        case 1:
            snake.update(ai.update());
            break;
        case 2: { // Update ai but let human do the moving
            ai.update();
            snake.update(getControl());
        } break;
        }

        renderer.drawColor({10, 0, 0, 255});
        renderer.clear();

        renderer.drawColor({0, 0, 0, 255});
        canvas.draw(renderer);

        renderer.present();

        std::this_thread::sleep_for(100ms);
    }

    return 0;
}
