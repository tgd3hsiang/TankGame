#pragma once

#include "GridPos.h"
#include <vector>

class TileMap;

// Returns cells from start to goal (inclusive), or empty if unreachable.
std::vector<GridPos> FindPathAStar(const TileMap& map, GridPos start, GridPos goal);
