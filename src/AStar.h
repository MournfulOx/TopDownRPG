#pragma once
#include <vector>

struct Vec2 {
    int x, y;
    bool operator==(const Vec2& o) const { return x == o.x && y == o.y; }
};

std::vector<Vec2> FindPath(
    Vec2 start,
    Vec2 goal,
    int gridW,
    int gridH,
    const std::vector<std::vector<int>>& grid
);
