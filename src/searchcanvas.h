#pragma once

#include "canvas.h"
#include "point.h"

namespace snake {

struct SearchCanvasCell {
    bool explored;
    SmallPoint parent;
};

struct SearchCanvas : public Canvas<SmallPoint> {};

} // namespace snake
