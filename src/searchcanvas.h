#pragma once

#include "canvas.h"
#include "point.h"

namespace snake {

struct SearchCanvasCell {
    bool explored = false;
    SmallPoint parent;
};

struct SearchCanvas : public Canvas<SearchCanvasCell> {
    void explore(Point to, Point from) {
        if (isInside(to) && !get(to).explored) {
            set(to,
                SearchCanvasCell{
                    true,
                    from,
                });
        }
    }
};

} // namespace snake
