#include "Player.h"
#include "raylib.h"

void Player::Update(float dt) {
    if (IsKeyDown(KEY_W)) y -= speed * dt;
    if (IsKeyDown(KEY_S)) y += speed * dt;
    if (IsKeyDown(KEY_A)) x -= speed * dt;
    if (IsKeyDown(KEY_D)) x += speed * dt;

    if (cooldownTimer > 0) cooldownTimer -= dt;
}

void Player::Draw() {
    DrawCircle((int)x, (int)y, 20, BLUE);
    DrawText(TextFormat("HP: %d", hp), (int)x - 15, (int)y - 35, 16, GREEN);
}

bool Player::CanAttack() {
    return cooldownTimer <= 0;
}

void Player::ResetCooldown() {
    cooldownTimer = attackCooldown;
}
