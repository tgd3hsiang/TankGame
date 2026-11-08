#include "Game.h"
#include <algorithm>

namespace {
bool Hit(Vector2 a, float ra, Vector2 b, float rb) {
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    return dx * dx + dy * dy < (ra + rb) * (ra + rb);
}
}

Game::Game()
    : player_({100, 100}, SKYBLUE), enemy_({640, 320}, RED) {
    InitWindow(TileMap::kCols * TileMap::kTileSize,
               TileMap::kRows * TileMap::kTileSize,
               "Tank Battle with Enemy AI");
    SetTargetFPS(60);
}

Game::~Game() { CloseWindow(); }

void Game::Reset() {
    player_ = Tank({100, 100}, SKYBLUE);
    enemy_ = Tank({640, 320}, RED);
    enemyAI_ = EnemyAI{};
    projectiles_.clear();
    state_ = GameState::Playing;
}

void Game::Tick() {
    if (IsKeyPressed(KEY_F1)) showDebug_ = !showDebug_;
    if (state_ != GameState::Playing && IsKeyPressed(KEY_R)) Reset();
    Update(std::min(GetFrameTime(), 0.033f));
    Draw();
}

void Game::Update(float dt) {
    if (state_ != GameState::Playing) return;

    player_.UpdateCooldown(dt);
    enemy_.UpdateCooldown(dt);

    Vector2 move{};
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) move.y -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) move.y += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) move.x -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) move.x += 1.0f;
    player_.Move(move, dt, map_);
    player_.AimAt(GetMousePosition());
    if ((IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsKeyDown(KEY_SPACE)) &&
        player_.CanShoot()) {
        projectiles_.push_back(player_.Shoot(Team::Player));
    }

    enemyAI_.Update(dt, enemy_, player_, map_, projectiles_);
    UpdateProjectiles(dt);
}

void Game::UpdateProjectiles(float dt) {
    for (Projectile& bullet : projectiles_) {
        if (!bullet.active) continue;
        bullet.Update(dt);
        if (map_.IsCircleBlocked(bullet.position, bullet.radius)) {
            bullet.active = false;
            continue;
        }
        if (bullet.owner == Team::Player && enemy_.IsAlive() &&
            Hit(bullet.position, bullet.radius, enemy_.Position(), enemy_.Radius())) {
            enemy_.Destroy();
            state_ = GameState::Won;
            bullet.active = false;
        } else if (bullet.owner == Team::Enemy && player_.IsAlive() &&
                   Hit(bullet.position, bullet.radius, player_.Position(), player_.Radius())) {
            player_.Destroy();
            state_ = GameState::Lost;
            bullet.active = false;
        }
    }
    projectiles_.erase(
        std::remove_if(projectiles_.begin(), projectiles_.end(),
                       [](const Projectile& p) { return !p.active; }),
        projectiles_.end());
}

void Game::Draw() const {
    BeginDrawing();
    ClearBackground(BLACK);
    map_.Draw(showDebug_);
    for (const Projectile& bullet : projectiles_) bullet.Draw();
    player_.Draw();
    enemy_.Draw();
    if (showDebug_ && enemy_.IsAlive()) enemyAI_.DrawDebug(enemy_, map_);

    DrawText("WASD / ARROWS: move   MOUSE: aim   CLICK / SPACE: fire",
             10, 570, 15, RAYWHITE);
    DrawText("F1: AI debug   R: restart after game over", 10, 550, 15, LIGHTGRAY);
    if (state_ != GameState::Playing) {
        DrawRectangle(150, 220, 500, 140, Color{0, 0, 0, 220});
        DrawText(state_ == GameState::Won ? "YOU WIN" : "GAME OVER",
                 285, 255, 38, state_ == GameState::Won ? GREEN : RED);
        DrawText("Press R to restart", 305, 312, 18, RAYWHITE);
    }
    EndDrawing();
}
