#pragma once

#include <unordered_map>
#include <unordered_set>
#include "router_helpers.hpp"
namespace router
{
class Router
{
public:
    Router(int x, int y, Coord start);
    void navigate(std::vector<Request> const&);
    std::vector<State> get_neighbors(State const& curr) const noexcept;

private:
    const int _map_x{0};
    const int _map_y{0};
    State _curr_state;
    Route _curr_route;
    std::unordered_set<State, State_hash, State_compare> _visited{};
    std::unordered_map<int, Request> _requests{};
    std::unordered_map<int, std::vector<int>> _pickups{};
    std::unordered_map<int, std::vector<int>> _dropoffs{};

    inline bool is_valid(Coord const& coord) const noexcept
    {
        return coord.x >= 0 && coord.y >= 0 && coord.x < _map_x &&
               coord.y < _map_y;
    }

    inline int coord_to_idx(Coord const& coord) const noexcept
    {
        return coord.y * _map_x + coord.x;
    }

    Route BFS();
};

}  // namespace router
