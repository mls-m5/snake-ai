#include "obstaclecanvas.h"
#include <random>

namespace {

auto generator = std::mt19937{std::random_device{}()};

}

namespace snake {

void ObstacleCanvas::draw(sdl::RendererView renderer) {
    renderer.drawColor({0, 20, 0, 255});
    renderer.fillRect({0, 0, width * cellSize, height * cellSize});

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto d = get(x, y);
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

void ObstacleCanvas::putApple() {
    int numPossibleCells = 0;
    for (auto cell : data()) {
        if (!cell) {
            ++numPossibleCells;
        }
    }

    if (!numPossibleCells) {
        return; // Nowhere to put apple
    }

    auto dist = std::uniform_int_distribution{0, numPossibleCells - 1};
    auto rnd = dist(generator);

    applePos = [&] {
        for (size_t i = 0; i < data().size(); ++i) {
            if (!data().at(i)) {
                if (rnd == 0) {
                    return pointFromIndex(i);
                }
                --rnd;
            }
        }
        return Point{};
    }();

    set(applePos, ObstacleCanvas::Apple);
}

} // namespace snake
