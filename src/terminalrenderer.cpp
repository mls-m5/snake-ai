#include "terminalrenderer.h"
#include "obstaclecanvas.h"

namespace snake {

TerminalRenderer::TerminalRenderer(const Settings &settings)
    : settings{settings} {}

void TerminalRenderer::beginDraw() {}

void TerminalRenderer::finishDraw() {}

void TerminalRenderer::draw(const ObstacleCanvas &canvas) {
    for (int y = 0; y < canvas.height; ++y) {
        for (int x = 0; x < canvas.width; ++x) {
            //            auto s = std::to_string(canvas.at(x, y));
            std::cout << " " << canvas.at(x, y);
        }
        std::cout << "\n";
    }

    std::cout.flush();
}

} // namespace snake
