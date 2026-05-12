#pragma once
#include "AStar.h"
#include <vector>

enum class EnemyState {
    Idle,
    Chase,
    Attack,
    Dead
};

struct Enemy {
    float x, y;
    float speed;
    int hp;
    EnemyState state;

    std::vector<Vec2> path;
    int pathIndex;
    float pathTimer;

    void Update(float dt, float playerX, float playerY,
                const std::vector<std::vector<int>>& grid,
                int gridW, int gridH, int tileSize);
    void Draw();
};
