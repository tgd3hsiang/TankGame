#pragma once

#include "GridPos.h"
#include "raylib.h"
#include <array>
#include <string>

class TileMap {
public:
    static constexpr int kCols = 20;
    static constexpr int kRows = 15;
    static constexpr int kTileSize = 40;

    TileMap();

    bool IsWall(GridPos cell) const;
    bool IsCircleBlocked(Vector2 center, float radius) const;
    bool HasLineOfSight(Vector2 from, Vector2 to) const;
    GridPos WorldToCell(Vector2 position) const;
    Vector2 CellCenter(GridPos cell) const;
    void Draw(bool showGrid) const;

private:
    std::array<std::string, kRows> rows_;
};
