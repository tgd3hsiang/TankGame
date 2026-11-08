#include "TileMap.h"
#include <cmath>

TileMap::TileMap() : rows_{{
    "####################",
    "#..................#",
    "#.....##...........#",
    "#.....##......##...#",
    "#.............##...#",
    "#..###.............#",
    "#..###...####......#",
    "#.........##.......#",
    "#.....##...........#",
    "#.....##.....###...#",
    "#............###...#",
    "#...####...........#",
    "#..................#",
    "#..................#",
    "####################"
}} {}

bool TileMap::IsWall(GridPos cell) const {
    if (cell.x < 0 || cell.y < 0 || cell.x >= kCols || cell.y >= kRows) {
        return true;
    }
    return rows_[cell.y][cell.x] == '#';
}

GridPos TileMap::WorldToCell(Vector2 position) const {
    return {static_cast<int>(std::floor(position.x / kTileSize)),
            static_cast<int>(std::floor(position.y / kTileSize))};
}

Vector2 TileMap::CellCenter(GridPos cell) const {
    return {(cell.x + 0.5f) * kTileSize,
            (cell.y + 0.5f) * kTileSize};
}

bool TileMap::IsCircleBlocked(Vector2 center, float radius) const {
    if (center.x - radius < 0 || center.y - radius < 0 ||
        center.x + radius >= kCols * kTileSize ||
        center.y + radius >= kRows * kTileSize) {
        return true;
    }
    const GridPos topLeft = WorldToCell({center.x - radius, center.y - radius});
    const GridPos bottomRight = WorldToCell({center.x + radius, center.y + radius});
    for (int y = topLeft.y; y <= bottomRight.y; ++y) {
        for (int x = topLeft.x; x <= bottomRight.x; ++x) {
            if (IsWall({x, y})) {
                Rectangle wall{float(x * kTileSize), float(y * kTileSize),
                               float(kTileSize), float(kTileSize)};
                if (CheckCollisionCircleRec(center, radius, wall)) return true;
            }
        }
    }
    return false;
}

bool TileMap::HasLineOfSight(Vector2 from, Vector2 to) const {
    const float dx = to.x - from.x;
    const float dy = to.y - from.y;
    const float length = std::sqrt(dx * dx + dy * dy);
    const int steps = static_cast<int>(length / 8.0f) + 1;
    for (int i = 0; i <= steps; ++i) {
        const float t = float(i) / float(steps);
        if (IsWall(WorldToCell({from.x + t * dx, from.y + t * dy}))) {
            return false;
        }
    }
    return true;
}

void TileMap::Draw(bool showGrid) const {
    for (int y = 0; y < kRows; ++y) {
        for (int x = 0; x < kCols; ++x) {
            const bool wall = IsWall({x, y});
            DrawRectangle(x * kTileSize, y * kTileSize,
                          kTileSize, kTileSize,
                          wall ? Color{43, 57, 72, 255} : Color{18, 27, 37, 255});
            if (wall) {
                DrawRectangleLines(x * kTileSize, y * kTileSize,
                                   kTileSize, kTileSize, Color{67, 90, 110, 255});
            } else if (showGrid) {
                DrawRectangleLines(x * kTileSize, y * kTileSize,
                                   kTileSize, kTileSize, Color{39, 52, 63, 255});
            }
        }
    }
}
