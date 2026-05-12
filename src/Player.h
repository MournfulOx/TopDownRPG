#pragma once

struct Player {
    float x, y;
    float speed;
    int hp;
    float attackRange;
    float attackCooldown;
    float cooldownTimer;

    void Update(float dt);
    void Draw();
    bool CanAttack();
    void ResetCooldown();
};
