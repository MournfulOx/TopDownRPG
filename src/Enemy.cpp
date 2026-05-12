#include "Enemy.h"
#include "raylib.h"
#include <cmath>

static float Dist(float x1, float y1, float x2, float y2) {
    float dx = x2-x1, dy = y2-y1;
    return std::sqrt(dx*dx + dy*dy);
}

void Enemy::Update(float dt, float playerX, float playerY,
                   const std::vector<std::vector<int>>& grid,
                   int gridW, int gridH, int tileSize) {

    if (state == EnemyState::Dead) return;

    float dist = Dist(x, y, playerX, playerY);

    if (dist < 40.0f) {
        state = EnemyState::Attack;
        return;
    } else if (dist < 200.0f) {
        state = EnemyState::Chase;
    } else {
        state = EnemyState::Idle;
        return;
    }

    // Chase：重算路径
    pathTimer -= dt;
    if (pathTimer <= 0 || path.empty()) {
        pathTimer = 0.5f;

        Vec2 start = {(int)(x / tileSize), (int)(y / tileSize)};
        Vec2 goal  = {(int)(playerX / tileSize), (int)(playerY / tileSize)};

        path = FindPath(start, goal, gridW, gridH, grid);
        pathIndex = (int)path.size() - 1;
    }

    // 沿路径移动
    if (!path.empty() && pathIndex >= 0) {
        Vec2 target = path[pathIndex];
        float tx = target.x * tileSize + tileSize / 2.0f;
        float ty = target.y * tileSize + tileSize / 2.0f;

        float dx = tx - x;
        float dy = ty - y;
        float len = std::sqrt(dx*dx + dy*dy);

        if (len < 4.0f) {
            pathIndex--;
        } else {
            x += (dx/len) * speed * dt;
            y += (dy/len) * speed * dt;
        }
    }
}

void Enemy::Draw() {
    if (state == EnemyState::Dead) return;
    Color c = (state == EnemyState::Attack) ? RED : ORANGE;
    DrawCircle((int)x, (int)y, 18, c);
    DrawText(TextFormat("HP:%d", hp), (int)x-15, (int)y-35, 16, WHITE);
}
