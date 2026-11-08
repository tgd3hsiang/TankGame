#pragma once

#include "EnemyAI.h"
#include "Projectile.h"
#include "Tank.h"
#include "TileMap.h"
#include <vector>

enum class GameState { Playing, Won, Lost };

class Game {
public:
    Game();
    ~Game();
    void Tick(); // Called by desktop loop or Emscripten callback.

private:
    void Reset();
    void Update(float dt);
    void UpdateProjectiles(float dt);
    void Draw() const;

    TileMap map_;
    Tank player_;
    Tank enemy_;
    EnemyAI enemyAI_;
    std::vector<Projectile> projectiles_;
    GameState state_ = GameState::Playing;
    bool showDebug_ = true;
};
