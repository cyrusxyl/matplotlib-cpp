#include "router.hpp"
#include <iostream>

namespace router
{
Router::Router(int x, int y, Coord start)
    : _map_x{x}, _map_y{y}, _curr_state{start, {}, {}, {}}
{
}

void Router::navigate(std::vector<Request> const& requests)
{
    if (requests.empty())
    {
        // update_state(_curr_route.front());
        // _curr_route.pop();
        return;
    }
    for (auto const& request : requests)
    {
        _requests[request.id] = request;
        _pickups[coord_to_idx(request.pickup)].push_back(request.id);
        _dropoffs[coord_to_idx(request.dropoff)].push_back(request.id);
    }
    if (_requests.empty())
    {
        return;
    }
    _curr_route = BFS();
    // for (auto const& c : _curr_route)
    // {
    //     std::cout << '(' << c.x << ',' << c.y << ")\n";
    // }
}

Route Router::BFS()
{
    std::queue<State> queue;
    queue.push(_curr_state);
    while (!queue.empty())
    {
        auto cs = queue.front();
        if (cs.fullfilled.size() == _requests.size())
        {
            return cs.route;
        }
        queue.pop();
        _visited.insert(cs);
        for (auto const& ns : get_neighbors(cs))
        {
            queue.push(ns);
        }
    }
    return {};
}

std::vector<State> Router::get_neighbors(const State& curr) const noexcept
{
    std::vector<State> neighbors;
    for (auto const& m : router::MOVES)
    {
        auto new_location = curr.car_location.move(m);
        if (!is_valid(new_location))
        {
            continue;
        }
        State new_state{new_location, curr.passenges, curr.fullfilled,
                        curr.route};
        new_state.route.push(new_location);
        int new_idx = coord_to_idx(new_location);
        auto pickup_it = _pickups.find(new_idx);
        if (pickup_it != _pickups.end())
        {
            for (auto id : pickup_it->second)
            {
                if (new_state.fullfilled.find(id) == new_state.fullfilled.end())
                {
                    new_state.passenges.insert(id);
                }
            }
        }
        auto dropoff_it = _dropoffs.find(new_idx);
        if (dropoff_it != _dropoffs.end())
        {
            for (auto id : dropoff_it->second)
            {
                if (new_state.passenges.erase(id) != 0)
                {
                    new_state.fullfilled.insert(id);
                }
            }
        }
        if (_visited.find(new_state) != _visited.end())
        {
            continue;
        }
        neighbors.push_back(new_state);
    }
    return neighbors;
}

}  // namespace router
