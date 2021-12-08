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

void SdlRenderer::draw(const Snake &snake) {
    auto offset = cellSize / 2;
    renderer.drawColor(0, 100, 0);

    auto head = snake.head();
    renderer.fillRect(
        {head.x * cellSize, head.y * cellSize, cellSize, cellSize});

    renderer.drawColor(0, 0, 0);

    auto drawLine = [&](Point from, Point to) {
        renderer.drawLine(from.x * cellSize + offset,
                          from.y * cellSize + offset,
                          to.x * cellSize + offset,
                          to.y * cellSize + offset);
    };

    auto prev = snake.segments().front();
    auto begin = std::next(snake.segments().begin());
    for (auto it = begin; it != snake.segments().end(); ++it) {
        drawLine(*it, prev);
        prev = *it;
    }
}

void SdlRenderer::draw(const Ai &ai) {
    loopPrevention.clear();
    auto offset = cellSize / 2;

    renderer.drawColor({80, 80, 80, 255});
    if (settings.shouldShowPath || settings.shouldShowSearch) {
        for (auto t : ai.tailDelay()) {
            renderer.fillRect(
                {t.x * cellSize, t.y * cellSize, cellSize / 2, cellSize / 2});
        }
    }

    renderer.drawColor({100, 100, 100, 255});

    auto drawLine = [&](Point from, Point to) {
        renderer.drawLine(from.x * cellSize + offset,
                          from.y * cellSize + offset,
                          to.x * cellSize + offset,
                          to.y * cellSize + offset);
    };

    if (settings.shouldShowSearch) {
        draw(ai.searchCanvas(), 0);
        draw(ai.returnSearchCanvas(), 1);
    }

    if (settings.shouldShowPath) {
        auto &searchCanvas = ai.searchCanvas();

        // Draw the real path to apple

        auto backtrackDraw = [&](Point pos, const SearchCanvas &searchCanvas) {
            renderer.drawColor({255, 255, 255, 255});
            for (SearchCanvasCell cell;
                 cell = searchCanvas.at(pos), cell.isRenderable();
                 pos = cell.parent) {
                if (loopPrevention.at(pos)) {
                    break;
                }
                loopPrevention.set(pos, true);
                drawLine(pos, cell.parent);
            }
        };
        backtrackDraw(ai.obstacleCanvas().applePos, ai.searchCanvas());
        backtrackDraw(ai.trackedTail(), ai.returnSearchCanvas());
    }
}

void SdlRenderer::draw(const SearchCanvas &canvas, int duplicationOffset) {
    const auto offset = cellSize / 2 + duplicationOffset;
    for (int y = 0; y < canvas.height; ++y) {
        for (int x = 0; x < canvas.width; ++x) {
            auto cell = canvas.at(x, y);
            if (cell.isRenderable()) {
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
