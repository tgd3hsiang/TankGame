#include "Projectile.h"

void Projectile::Update(float dt) {
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
}

void Projectile::Draw() const {
    if (active) {
        DrawCircleV(position, radius, owner == Team::Player ? GOLD : ORANGE);
    }
}
