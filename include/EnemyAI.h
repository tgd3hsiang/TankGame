#pragma once

#include "GridPos.h"
#include "Projectile.h"
#include <vector>

class Tank;
class TileMap;

enum class EnemyState { Patrol, Chase, Attack };

class EnemyAI {
public:
    void Update(float dt, Tank& enemy, const Tank& player,
                const TileMap& map, std::vector<Projectile>& projectiles);
    void DrawDebug(const Tank& enemy, const TileMap& map) const;
    const char* StateName() const;

private:
    EnemyState state_ = EnemyState::Patrol;
    std::vector<GridPos> path_;
    float repathTimer_ = 0.0f;
    int patrolIndex_ = 0;
};
