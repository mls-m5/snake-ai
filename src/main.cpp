

#include "sdlpp/render.hpp"
#include "sdlpp/surface.hpp"
#include "sdlpp/window.hpp"
#include <array>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

struct Canvas {
    static constexpr int width = 32;
    static constexpr int height = 32;
    int cellSize = 10;
    std::array<int, width *height> data = {};

    void draw(sdl::RendererView renderer) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                auto d = data.at(x + y * width);
                if (d) {
                    if (d == 1) {
                        renderer.drawColor({100, 100, 100, 255});
                    }
                    else if (d == 2) {
                        renderer.drawColor({255, 0, 0, 255});
                    }
                    renderer.fillRect(
                        {x * cellSize, y * cellSize, cellSize, cellSize});
                }
            }
        }
    }

    auto &at(int x, int y) {
        return data.at(y * width + x);
    }
};

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

    renderer.drawColor({100, 0, 0, 255});
    renderer.clear();
    renderer.drawColor({100, 100, 100, 255});
    renderer.drawLine(0, 0, 100, 100);

    auto canvas = Canvas{};

    renderer.drawColor({0, 0, 0, 255});
    canvas.at(1, 1) = 1;
    canvas.at(2, 1) = 1;
    canvas.draw(renderer);

    renderer.present();

    std::this_thread::sleep_for(1000ms);

    return 0;
}
