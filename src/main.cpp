#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include <algorithm>
#include <cmath>
#include <vector>

const int TILE = 40;
const int COLS = 20;
const int ROWS = 15;

int main() {
    InitWindow(COLS*TILE, ROWS*TILE, "RPG Demo");
    SetTargetFPS(60);

    std::vector<std::vector<int>> grid(ROWS, std::vector<int>(COLS, 0));
    grid[5][5] = grid[5][6] = grid[5][7] = grid[5][8] = 1;
    grid[8][10] = grid[9][10] = grid[10][10] = 1;

    Player player = {400, 300, 200.0f, 100, 80.0f, 0.5f, 0};
    std::vector<Enemy> enemies = {
        {200, 200, 120.0f, 50, EnemyState::Idle, {}, 0, 0},
        {600, 400, 100.0f, 50, EnemyState::Idle, {}, 0, 0},
    };
    bool gameOver = false;
    bool youWin   = false;
    float dmgAccum = 0.0f; // 累积伤害，解决 dt 截断为 0 的问题

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (!gameOver && !youWin) {
            player.Update(dt);

            for (auto& e : enemies)
                e.Update(dt, player.x, player.y, grid, COLS, ROWS, TILE);

            // 玩家攻击
            if (IsKeyPressed(KEY_SPACE) && player.CanAttack()) {
                for (auto& e : enemies) {
                    if (e.state == EnemyState::Dead) continue;
                    float dx = e.x - player.x;
                    float dy = e.y - player.y;
                    if (std::sqrt(dx*dx + dy*dy) <= player.attackRange) {
                        e.hp -= 20;
                        if (e.hp <= 0) e.state = EnemyState::Dead;
                        player.ResetCooldown();
                        break;
                    }
                }
            }

            // 敌人伤害玩家（累积后取整，避免 dt*10 截断为 0）
            for (auto& e : enemies)
                if (e.state == EnemyState::Attack)
                    dmgAccum += 10.0f * dt;
            if (dmgAccum >= 1.0f) {
                player.hp -= (int)dmgAccum;
                dmgAccum -= (int)dmgAccum;
            }

            // 胜负判定
            if (player.hp <= 0) gameOver = true;
            youWin = std::all_of(enemies.begin(), enemies.end(),
                [](const Enemy& e){ return e.state == EnemyState::Dead; });
        }

        // 重置
        if (IsKeyPressed(KEY_R)) {
            player = {400, 300, 200.0f, 100, 80.0f, 0.5f, 0};
            enemies = {
                {200, 200, 120.0f, 50, EnemyState::Idle, {}, 0, 0},
                {600, 400, 100.0f, 50, EnemyState::Idle, {}, 0, 0},
            };
            gameOver  = false;
            youWin    = false;
            dmgAccum  = 0.0f;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (int r = 0; r < ROWS; r++)
            for (int c = 0; c < COLS; c++)
                if (grid[r][c] == 1)
                    DrawRectangle(c*TILE, r*TILE, TILE, TILE, DARKGRAY);

        DrawCircleLines((int)player.x, (int)player.y, player.attackRange, YELLOW);
        player.Draw();
        for (auto& e : enemies) e.Draw();
        DrawText(TextFormat("Player HP: %d", player.hp), 10, 10, 20, WHITE);

        if (gameOver) {
            DrawRectangle(0, 0, COLS*TILE, ROWS*TILE, {0, 0, 0, 160});
            DrawText("GAME OVER", 220, 240, 60, RED);
            DrawText("Press R to restart", 270, 320, 24, WHITE);
        }
        if (youWin) {
            DrawRectangle(0, 0, COLS*TILE, ROWS*TILE, {0, 0, 0, 160});
            DrawText("YOU WIN", 270, 240, 60, GREEN);
            DrawText("Press R to restart", 270, 320, 24, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
