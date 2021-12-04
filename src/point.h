#pragma once

struct Point {
    int x, y;

    void operator+=(const Point &other) {
        x += other.x;
        y += other.y;
    }
};
