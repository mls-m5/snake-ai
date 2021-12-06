#include "obstaclecanvas.h"
#include <random>

namespace {

auto generator = std::mt19937{std::random_device{}()};

}

namespace snake {

void ObstacleCanvas::putApple() {
    int numPossibleCells = 0;
    for (auto cell : data()) {
        if (!cell) {
            ++numPossibleCells;
        }
    }

    auto dist = std::uniform_int_distribution{0, numPossibleCells - 1};
    auto rnd = dist(generator);

    auto applePos = [&] {
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
