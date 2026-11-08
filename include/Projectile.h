#pragma once

#include "raylib.h"

enum class Team { Player, Enemy };

struct Projectile {
    Vector2 position{};
    Vector2 velocity{};
    float radius = 5.0f;
    Team owner = Team::Player;
    bool active = true;

    void Update(float dt);
    void Draw() const;
};
