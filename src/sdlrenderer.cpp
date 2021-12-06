#include "sdlrenderer.h"
#include "ai.h"
#include "obstaclecanvas.h"
#include "searchcanvas.h"
#include "settings.h"
#include <iostream>

namespace snake {

SdlRenderer::SdlRenderer(const Settings &settings)
    : window{"snake ai",
             SDL_WINDOWPOS_CENTERED,
             SDL_WINDOWPOS_CENTERED,
             500,
             500,
             SDL_WINDOW_OPENGL}
    , renderer{window.get(), -1, SDL_RENDERER_ACCELERATED}
    , settings{settings} {

    if (!renderer) {
        std::cerr << "could not create renderer\n";
        exit(1);
    }
}

void SdlRenderer::color(int r, int g, int b) {
    renderer.drawColor(r, g, b);
}

void SdlRenderer::beginDraw() {
    renderer.drawColor({10, 0, 0, 255});
    renderer.clear();
}

void SdlRenderer::finishDraw() {
    renderer.present();
}

void SdlRenderer::draw(const ObstacleCanvas &canvas) {
    const auto width = canvas.width;
    const auto height = canvas.height;
    const auto cellSize = canvas.cellSize;

    renderer.drawColor({0, 20, 0, 255});
    renderer.fillRect({0, 0, width * cellSize, height * cellSize});

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto d = canvas.at(x, y);
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

void SdlRenderer::draw(const Ai &ai) {
    const auto cellSize = ai.obstacleCanvas().cellSize;
    auto offset = cellSize / 2;

    renderer.drawColor({100, 100, 100, 255});

    auto drawLine = [&](Point from, Point to) {
        renderer.drawLine(from.x * cellSize + offset,
                          from.y * cellSize + offset,
                          to.x * cellSize + offset,
                          to.y * cellSize + offset);
    };

    if (settings.shouldShowSearch) {
        draw(ai.searchCanvas(), cellSize);
        draw(ai.returnSearchCanvas(), cellSize, 1);
    }

    if (settings.shouldShowPath) {
        auto &searchCanvas = ai.searchCanvas();

        // Draw the real path to apple

        auto backtrackDraw = [&](Point pos, const SearchCanvas &searchCanvas) {
            renderer.drawColor({255, 255, 255, 255});
            for (SearchCanvasCell cell;
                 cell = searchCanvas.at(pos), cell.explored;
                 pos = cell.parent) {
                drawLine(pos, cell.parent);
            }
        };
        //        Point pos = ai.obstacleCanvas().applePos;
        backtrackDraw(ai.obstacleCanvas().applePos, ai.searchCanvas());
        backtrackDraw(ai.snake().tail(), ai.returnSearchCanvas());
    }
}

void SdlRenderer::draw(const SearchCanvas &canvas,
                       int cellSize,
                       int duplicationOffset) {
    const auto offset = cellSize / 2 + duplicationOffset;
    for (int y = 0; y < canvas.height; ++y) {
        for (int x = 0; x < canvas.width; ++x) {
            auto cell = canvas.at(x, y);
            if (cell.explored) {
                auto parent = cell.parent;
                renderer.drawLine(x * cellSize + offset,
                                  y * cellSize + offset,
                                  parent.x * cellSize + offset,
                                  parent.y * cellSize + offset);
            }
        }
    }
}

} // namespace snake
