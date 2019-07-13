#pragma once
#include <set>
#include "router_helpers.hpp"
namespace router
{
class Router
{
public:
    Router(int x, int y, Coord start);
    void navigate(std::vector<Request> const&);
    std::vector<Coord> get_neighbors(Coord const& curr);

private:
    const int _map_x{0};
    const int _map_y{0};
    State _curr_state;
    Route _curr_route;
};

}  // namespace router
