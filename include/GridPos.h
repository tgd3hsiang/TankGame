#pragma once

struct GridPos {
    int x = 0; // column
    int y = 0; // row

    bool operator==(const GridPos& other) const {
        return x == other.x && y == other.y;
    }
};
