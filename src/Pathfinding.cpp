#include "Pathfinding.h"
#include "TileMap.h"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <limits>
#include <queue>

namespace {
constexpr int kCount = TileMap::kCols * TileMap::kRows;
int Id(GridPos cell) { return cell.y * TileMap::kCols + cell.x; }
GridPos Cell(int id) { return {id % TileMap::kCols, id / TileMap::kCols}; }
int Heuristic(GridPos a, GridPos b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
struct Node { int id; int score; };
struct HigherScore {
    bool operator()(Node a, Node b) const { return a.score > b.score; }
};
}

std::vector<GridPos> FindPathAStar(const TileMap& map, GridPos start, GridPos goal) {
    if (map.IsWall(start) || map.IsWall(goal)) return {};

    constexpr int inf = std::numeric_limits<int>::max();
    std::array<int, kCount> dist;
    std::array<int, kCount> parent;
    dist.fill(inf);
    parent.fill(-1);

    const int startId = Id(start);
    const int goalId = Id(goal);
    std::priority_queue<Node, std::vector<Node>, HigherScore> open;
    dist[startId] = 0;
    open.push({startId, Heuristic(start, goal)});

    constexpr GridPos neighbors[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    while (!open.empty()) {
        const Node current = open.top();
        open.pop();
        if (current.score != dist[current.id] + Heuristic(Cell(current.id), goal)) continue;
        if (current.id == goalId) break;

        const GridPos at = Cell(current.id);
        for (auto d : neighbors) {
            GridPos next{at.x + d.x, at.y + d.y};
            if (map.IsWall(next)) continue;
            const int nextId = Id(next);
            if (dist[current.id] + 1 < dist[nextId]) {
                dist[nextId] = dist[current.id] + 1;
                parent[nextId] = current.id;
                open.push({nextId, dist[nextId] + Heuristic(next, goal)});
            }
        }
    }

    if (dist[goalId] == inf) return {};
    std::vector<GridPos> reversed;
    for (int id = goalId; id != -1; id = parent[id]) {
        reversed.push_back(Cell(id));
    }
    std::reverse(reversed.begin(), reversed.end());
    return reversed;
}
