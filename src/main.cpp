

#include "canvas.h"
#include "sdlpp/render.hpp"
#include "sdlpp/surface.hpp"
#include "sdlpp/window.hpp"
#include "snake.h"
#include <iostream>
#include <thread>

using namespace snake;

using namespace std::chrono_literals;

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

    auto canvas = Canvas{};
    auto snake = Snake{canvas};

    for (int i = 0; i < 4; ++i) {
        snake.update(0);

        renderer.drawColor({10, 0, 0, 255});
        renderer.clear();

        renderer.drawColor({0, 0, 0, 255});
        canvas.draw(renderer);

        renderer.present();

        std::this_thread::sleep_for(1000ms);
    }

    return 0;
}
