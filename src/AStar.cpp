#include "AStar.h"
#include <queue>
#include <unordered_map>
#include <cmath>

struct Node {
    Vec2 pos;
    float g, f;
    bool operator>(const Node& o) const { return f > o.f; }
};

static float Heuristic(Vec2 a, Vec2 b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

static int ToKey(Vec2 v, int gridW) {
    return v.y * gridW + v.x;
}

std::vector<Vec2> FindPath(Vec2 start, Vec2 goal, int gridW, int gridH,
                            const std::vector<std::vector<int>>& grid) {
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open;
    std::unordered_map<int, int> cameFrom;
    std::unordered_map<int, float> gScore;

    int startKey = ToKey(start, gridW);
    gScore[startKey] = 0;
    open.push({start, 0, Heuristic(start, goal)});

    const Vec2 dirs[] = {{0,-1},{0,1},{-1,0},{1,0}};

    while (!open.empty()) {
        Node current = open.top(); open.pop();

        if (current.pos == goal) {
            std::vector<Vec2> path;
            int key = ToKey(goal, gridW);
            while (cameFrom.count(key)) {
                int cx = key % gridW;
                int cy = key / gridW;
                path.push_back({cx, cy});
                key = cameFrom[key];
            }
            return path;
        }

        for (auto& d : dirs) {
            Vec2 next = {current.pos.x + d.x, current.pos.y + d.y};
            if (next.x < 0 || next.x >= gridW || next.y < 0 || next.y >= gridH) continue;
            if (grid[next.y][next.x] == 1) continue;

            int nextKey = ToKey(next, gridW);
            float newG = gScore[ToKey(current.pos, gridW)] + 1;

            if (!gScore.count(nextKey) || newG < gScore[nextKey]) {
                gScore[nextKey] = newG;
                float f = newG + Heuristic(next, goal);
                open.push({next, newG, f});
                cameFrom[nextKey] = ToKey(current.pos, gridW);
            }
        }
    }
    return {};
}
