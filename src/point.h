#pragma once

struct Point {
    int x = 0;
    int y = 0;

    void operator+=(Point other) {
        x += other.x;
        y += other.y;
    }

    Point operator+(Point other) {
        return {x + other.x, y + other.y};
    }
};
