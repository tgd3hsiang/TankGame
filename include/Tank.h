#pragma once

#include "Projectile.h"
#include "raylib.h"

class TileMap;

class Tank {
public:
    Tank(Vector2 startPosition, Color color);

    void UpdateCooldown(float dt);
    void Move(Vector2 direction, float dt, const TileMap& map);
    void AimAt(Vector2 worldTarget);
    bool CanShoot() const;
    Projectile Shoot(Team owner);
    void Draw() const;

    Vector2 Position() const { return position_; }
    float Radius() const { return radius_; }
    bool IsAlive() const { return alive_; }
    void Destroy() { alive_ = false; }

private:
    Vector2 position_{};
    Vector2 aim_{1.0f, 0.0f};
    Color color_{};
    float radius_ = 15.0f;
    float speed_ = 155.0f;
    float cooldown_ = 0.0f;
    float fireInterval_ = 0.55f;
    bool alive_ = true;
};
