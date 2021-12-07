#pragma once

#include "canvas.h"
#include "obstaclecanvas.h"
#include "point.h"

namespace snake {

struct SearchCanvasCell {
    bool explored = false;
    SmallPoint parent = {-1, -1};

    bool isRenderable() const {
        return explored && parent.x != -1;
    }

    bool isFree() const {
        return !explored;
    }
};

struct SearchCanvas : public Canvas<SearchCanvasCell> {
    enum ExploreResult {
        Failed = 0,
        Explored = 1,
        Apple = 2,
    };

    ExploreResult exploreCell(Point to,
                              Point from,
                              Point target,
                              const ObstacleCanvas obstacleCanvas) {
        if (to.x == target.x && to.y == target.y) {
            set(to,
                SearchCanvasCell{
                    true,
                    from,
                });

            return Apple;
        }
        if (isInside(to) && !at(to).explored) {
            auto obstacle = obstacleCanvas.at(to);
            if (obstacle == ObstacleCanvas::None) {
                set(to,
                    SearchCanvasCell{
                        true,
                        from,
                    });
                return Explored;
            }
        }
        return Failed;
    }
};

} // namespace snake
