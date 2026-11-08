#include "EnemyAI.h"
#include "Pathfinding.h"
#include "Tank.h"
#include "TileMap.h"
#include <cmath>

namespace {
float Dist(Vector2 a, Vector2 b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) +
                     (a.y - b.y) * (a.y - b.y));
}
Vector2 Direction(Vector2 from, Vector2 to) {
    return {to.x - from.x, to.y - from.y};
}
}

void EnemyAI::Update(float dt, Tank& enemy, const Tank& player,
                     const TileMap& map, std::vector<Projectile>& projectiles) {
    const float distance = Dist(enemy.Position(), player.Position());
    const bool visible = map.HasLineOfSight(enemy.Position(), player.Position());

    if (distance < 260.0f && visible) {
        state_ = EnemyState::Attack;
    } else if (distance < 750.0f) {
        state_ = EnemyState::Chase;
    } else {
        state_ = EnemyState::Patrol;
    }

    // Recompute path periodically, not every frame.
    repathTimer_ -= dt;
    if (repathTimer_ <= 0.0f) {
        const GridPos goal = state_ == EnemyState::Patrol
            ? (patrolIndex_ == 0 ? GridPos{16, 3} : GridPos{16, 12})
            : map.WorldToCell(player.Position());
        path_ = FindPathAStar(map, map.WorldToCell(enemy.Position()), goal);
        repathTimer_ = 0.35f;
    }

    if (state_ == EnemyState::Attack) {
        enemy.AimAt(player.Position());
        if (enemy.CanShoot()) projectiles.push_back(enemy.Shoot(Team::Enemy));
        return;
    }

    // Follow next path cell; do not move directly through walls.
    if (!path_.empty()) {
        const Vector2 next = map.CellCenter(path_.front());
        if (Dist(enemy.Position(), next) < 9.0f) path_.erase(path_.begin());
    }
    if (!path_.empty()) {
        const Vector2 target = map.CellCenter(path_.front());
        enemy.Move(Direction(enemy.Position(), target), dt, map);
        enemy.AimAt(target);
    }
    if (state_ == EnemyState::Patrol && path_.empty()) {
        patrolIndex_ = 1 - patrolIndex_;
    }
}

void EnemyAI::DrawDebug(const Tank& enemy, const TileMap& map) const {
    for (const GridPos& step : path_) {
        const Vector2 p = map.CellCenter(step);
        DrawCircleV(p, 4.0f, YELLOW);
    }
    DrawCircleLines(static_cast<int>(enemy.Position().x),
                    static_cast<int>(enemy.Position().y), 260.0f,
                    Color{250, 155, 80, 120});
    DrawText(TextFormat("AI: %s", StateName()), 10, 12, 18, YELLOW);
}

const char* EnemyAI::StateName() const {
    switch (state_) {
        case EnemyState::Patrol: return "PATROL";
        case EnemyState::Chase: return "CHASE";
        case EnemyState::Attack: return "ATTACK";
    }
    return "UNKNOWN";
}
