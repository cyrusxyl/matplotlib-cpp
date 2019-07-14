#include "router.hpp"
#include <iostream>

namespace router
{
Router::Router(int x, int y, Coord start) : _map_x{x}, _map_y{y}, _curr_state{start, {}, {}, {}} {}
Router::~Router()
{
    // finish current route when existing
    if (_curr_route.empty())
    {
        std::cout << "-----------------FINISHED----------------\n";
        return;
    }
    while (!_curr_route.empty())
    {
        std::cout << "\nFinishing route: length remaining = " << _curr_route.size() << '\n';
        auto coord = _curr_route.front();
        _curr_route.pop();
        update_state(_curr_state, coord, true);
    }
    std::cout << "-----------------FINISHED----------------\n";
}

// parse json, calls worker function
void Router::navigate_json(std::string const& json)
{
    if (json.empty())
    {
        navigate({});
        return;
    }
    auto requests = router::RequestFactory::make_requests(json);
    navigate(requests);
}

// main worker
void Router::navigate(std::vector<Request> const& requests)
{
    // if no new request
    if (requests.empty())
    {
        // and no existing route
        if (_curr_route.empty())
        {
            // stay put
            std::cout << "\nWaiting" << '\n';
            update_state(_curr_state, _curr_state.car_location, true);
            return;
        }
        // otherwise finish current route
        std::cout << "\nFinishing route: length remaining = " << _curr_route.size() << '\n';
        auto coord = _curr_route.front();
        _curr_route.pop();
        update_state(_curr_state, coord, true);
        return;
    }
    // if new request
    std::cout << "\nNew request: planning" << '\n';
    // update requests information
    for (auto const& request : requests)
    {
        _requests[request.id] = request;
        _pickups[coord_to_idx(request.pickup)].insert(request.id);
        _dropoffs[coord_to_idx(request.dropoff)].insert(request.id);
    }
    // replanning with Astar or BFS, Astar is much faster when route is longer
    _curr_route = Astar();  // BFS()
    auto coord = _curr_route.front();
    _curr_route.pop();
    update_state(_curr_state, coord, true);
    cleanup_request();
    // traverse(_curr_route);
}

void Router::cleanup_request()
{
    // for all fullfilled request
    for (auto id : _curr_state.fullfilled)
    {
        // remove from location and remove from request list
        auto pickup_location = coord_to_idx(_requests[id].pickup);
        auto dropoff_location = coord_to_idx(_requests[id].dropoff);
        _pickups[pickup_location].erase(id);
        _dropoffs[dropoff_location].erase(id);
        _requests.erase(id);
    }
    _curr_state.fullfilled = {};
}

void Router::perform_pickup(int location_idx,
                            std::set<int>& passengers,
                            std::set<int>& fullfilled,
                            bool print) const noexcept
{
    // a list to print out information
    std::vector<int> pickup_list;
    // if pickup exist at location
    auto pickup_it = _pickups.find(location_idx);
    if (pickup_it != _pickups.end())
    {
        for (auto id : pickup_it->second)
        {
            // and is not fullfilled
            if (fullfilled.find(id) == fullfilled.end())
            {
                // pickup
                passengers.insert(id);
                pickup_list.push_back(id);
            }
        }
    }
    // print if needed
    if (print)
    {
        for (auto id : pickup_list)
        {
            std::cout << "Pickup: " << _requests.at(id) << '\n';
        }
    }
}

void Router::perform_dropoff(int location_idx,
                             std::set<int>& passengers,
                             std::set<int>& fullfilled,
                             bool print) const noexcept
{
    std::vector<int> dropoff_list;
    // if dropoff exist at location
    auto dropoff_it = _dropoffs.find(location_idx);
    if (dropoff_it != _dropoffs.end())
    {
        for (auto id : dropoff_it->second)
        {
            // and is a passenger already
            if (passengers.erase(id) != 0)
            {
                // dropoff
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

void Router::update_state(State& state, Coord const& coord, bool print) const noexcept
{
    // move car to location
    state.car_location = coord;
    if (print)
    {
        std::cout << "-----------------------------------------\n";
        std::cout << "Car location:" << state.car_location << '\n';
    }
    // perform actions if needed
    int idx = coord_to_idx(coord);
    perform_pickup(idx, state.passengers, state.fullfilled, print);
    perform_dropoff(idx, state.passengers, state.fullfilled, print);
    if (print)
    {
        std::cout << "Passenger:\n";
        for (auto id : state.passengers)
        {
            std::cout << '\t' << _requests.at(id) << '\n';
        }
        std::cout << "-----------------------------------------\n";
    }
}

// very ordinary BFS, equivalent to Dijkstra in this grid world
Route Router::BFS() const
{
    _visited = {};
    std::queue<State> queue;
    // in case we can pickup right away, update at current location
    auto copy_state = _curr_state;
    copy_state.route.push(copy_state.car_location);
    update_state(copy_state, copy_state.car_location);
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

// very ordinary Astar
Route Router::Astar() const
{
    _visited = {};
    auto compare = [this](State const& left, State const& right) -> bool {
        return (static_cast<int>(left.route.size()) + h_cost(left)) >
               (static_cast<int>(right.route.size()) + h_cost(right));
    };
    std::priority_queue<State, std::vector<State>, std::function<bool(State, State)>> queue(
        compare);
    // in case we can pickup right away, update at current location
    auto copy_state = _curr_state;
    copy_state.route.push(copy_state.car_location);
    update_state(copy_state, copy_state.car_location);
    queue.push(copy_state);
    while (!queue.empty())
    {
        auto cs = queue.top();
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

// needs to be admissibile to get optimum
int Router::h_cost(State const& state) const noexcept
{
    int h = 0;
    // for all requests
    for (auto const& kvpair : _requests)
    {
        auto const& id = kvpair.first;
        auto const& request = kvpair.second;
        // if fullfilled, no cost
        if (state.fullfilled.find(id) != state.fullfilled.end())
        {
            continue;
        }
        // if picked up, take maximum of distance to goal
        if (state.passengers.find(id) != state.passengers.end())
        {
            h = std::max(h, state.car_location.distance(request.dropoff));
            continue;
        }
        // if not picked up, take maximum of distance to pickup plus minimum route length
        h = std::max(h, state.car_location.distance(request.pickup) +
                            request.pickup.distance(request.dropoff));
    }
    // h is the minimum length finish the longest request
    return h;
}

std::vector<State> Router::get_neighbors(const State& curr) const noexcept
{
    std::vector<State> neighbors;
    // for each move
    for (auto const& m : router::MOVES)
    {
        auto new_location = curr.car_location.move(m);
        // disgard if out of bound
        if (!is_valid(new_location))
        {
            continue;
        }
        State new_state{new_location, curr.passengers, curr.fullfilled, curr.route};
        new_state.route.push(new_location);
        update_state(new_state, new_location);
        // disgard if seen, see State_hash and State_compare for more details
        if (_visited.find(new_state) != _visited.end())
        {
            continue;
        }
        neighbors.push_back(new_state);
    }
    return neighbors;
}

}  // namespace router
