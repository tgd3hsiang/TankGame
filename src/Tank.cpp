#include "Tank.h"
#include "TileMap.h"
#include <cmath>

namespace {
Vector2 NormalizeOrZero(Vector2 v) {
    const float length = std::sqrt(v.x * v.x + v.y * v.y);
    return length > 0.001f ? Vector2{v.x / length, v.y / length} : Vector2{0, 0};
}
}

Tank::Tank(Vector2 startPosition, Color color)
    : position_(startPosition), color_(color) {}

void Tank::UpdateCooldown(float dt) {
    if (cooldown_ > 0) cooldown_ -= dt;
}

void Tank::Move(Vector2 direction, float dt, const TileMap& map) {
    direction = NormalizeOrZero(direction);
    Vector2 xMove{position_.x + direction.x * speed_ * dt, position_.y};
    if (!map.IsCircleBlocked(xMove, radius_)) position_.x = xMove.x;
    Vector2 yMove{position_.x, position_.y + direction.y * speed_ * dt};
    if (!map.IsCircleBlocked(yMove, radius_)) position_.y = yMove.y;
}

void Tank::AimAt(Vector2 target) {
    const Vector2 direction{target.x - position_.x, target.y - position_.y};
    const Vector2 normalized = NormalizeOrZero(direction);
    if (normalized.x != 0 || normalized.y != 0) aim_ = normalized;
}

bool Tank::CanShoot() const { return alive_ && cooldown_ <= 0; }

Projectile Tank::Shoot(Team owner) {
    cooldown_ = fireInterval_;
    return {{position_.x + aim_.x * (radius_ + 10),
             position_.y + aim_.y * (radius_ + 10)},
            {aim_.x * 420.0f, aim_.y * 420.0f}, 5.0f, owner, true};
}

void Tank::Draw() const {
    if (!alive_) return;
    DrawCircleV(position_, radius_ + 2.0f, BLACK);
    DrawCircleV(position_, radius_, color_);
    DrawLineEx(position_, {position_.x + aim_.x * 26,
                           position_.y + aim_.y * 26}, 7.0f, RAYWHITE);
    DrawCircleV(position_, 5.0f, DARKGRAY);
}
