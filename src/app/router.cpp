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
        if (_curr_route.empty())
        {
            std::cout << "\nWaiting" << '\n';
            update_state(_curr_state, _curr_state.car_location);
            return;
        }
        std::cout << "\nFinishing route: length remaining = "
                  << _curr_route.size() << '\n';
        auto coord = _curr_route.front();
        _curr_route.pop();
        update_state(_curr_state, coord);
        return;
    }
    std::cout << "\nNew request" << '\n';
    for (auto const& request : requests)
    {
        _requests[request.id] = request;
        _pickups[coord_to_idx(request.pickup)].insert(request.id);
        _dropoffs[coord_to_idx(request.dropoff)].insert(request.id);
    }
    _visited = {};
    _curr_route = BFS();
    auto coord = _curr_route.front();
    _curr_route.pop();
    update_state(_curr_state, coord);
    cleanup_request();
    // traverse(_curr_route);
}

void Router::cleanup_request()
{
    for (auto id : _curr_state.fullfilled)
    {
        auto pickup_location = coord_to_idx(_requests[id].pickup);
        auto dropoff_location = coord_to_idx(_requests[id].dropoff);
        _pickups[pickup_location].erase(id);
        _dropoffs[dropoff_location].erase(id);
        _requests.erase(id);
    }
    _curr_state.fullfilled = {};
}

void Router::perform_pickup(int location_idx,
                            std::set<int>& passenges,
                            std::set<int>& fullfilled,
                            bool print) const noexcept
{
    std::vector<int> pickup_list;
    auto pickup_it = _pickups.find(location_idx);
    if (pickup_it != _pickups.end())
    {
        for (auto id : pickup_it->second)
        {
            if (fullfilled.find(id) == fullfilled.end())
            {
                passenges.insert(id);
                pickup_list.push_back(id);
            }
        }
    }
    if (print)
    {
        for (auto id : pickup_list)
        {
            std::cout << "Pickup: " << _requests.at(id) << '\n';
        }
    }
}

void Router::perform_dropoff(int location_idx,
                             std::set<int>& passenges,
                             std::set<int>& fullfilled,
                             bool print) const noexcept
{
    std::vector<int> dropoff_list;
    auto dropoff_it = _dropoffs.find(location_idx);
    if (dropoff_it != _dropoffs.end())
    {
        for (auto id : dropoff_it->second)
        {
            if (passenges.erase(id) != 0)
            {
                fullfilled.insert(id);
                dropoff_list.push_back(id);
            }
        }
    }
    if (print)
    {
        for (auto id : dropoff_list)
        {
            std::cout << "Droffoff: " << _requests.at(id) << '\n';
        }
    }
}

Router::~Router()
{
    if (_curr_route.empty())
    {
        std::cout << "-----------------FINISHED----------------\n";
    }
    while (!_curr_route.empty())
    {
        std::cout << "\nFinishing route: length remaining = "
                  << _curr_route.size() << '\n';
        auto coord = _curr_route.front();
        _curr_route.pop();
        update_state(_curr_state, coord);
    }
    std::cout << "-----------------FINISHED----------------\n";
}

void Router::update_state(State& state, Coord const& coord)
{
    state.car_location = coord;
    int idx = coord_to_idx(coord);
    std::cout << "-----------------------------------------\n";
    std::cout << "Car location:" << state.car_location << '\n';
    perform_pickup(idx, state.passenges, state.fullfilled, true);
    perform_dropoff(idx, state.passenges, state.fullfilled, true);
    std::cout << "Passenger:\n";
    for (auto id : state.passenges)
    {
        std::cout << '\t' << _requests.at(id) << '\n';
    }
    std::cout << "-----------------------------------------\n";
}

Route Router::BFS()
{
    std::queue<State> queue;
    // in case we can pickup right away
    int idx = coord_to_idx(_curr_state.car_location);
    auto copy_state = _curr_state;
    perform_pickup(idx, copy_state.passenges, copy_state.fullfilled);
    perform_dropoff(idx, copy_state.passenges, copy_state.fullfilled);
    copy_state.route.push(copy_state.car_location);
    queue.push(copy_state);
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
    throw std::runtime_error("no route found");
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
        perform_pickup(new_idx, new_state.passenges, new_state.fullfilled);
        perform_dropoff(new_idx, new_state.passenges, new_state.fullfilled);
        if (_visited.find(new_state) != _visited.end())
        {
            continue;
        }
        neighbors.push_back(new_state);
    }
    return neighbors;
}

}  // namespace router
