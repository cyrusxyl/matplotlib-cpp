// router main functionalities
#pragma once

#include <unordered_map>
#include <unordered_set>
#include "router_helpers.hpp"
namespace router
{
// router that takes a map size and starting position
class Router
{
public:
    Router(int x, int y, Coord start);
    ~Router();
    // given json element as string perform routing, and step once
    void navigate_json(std::string const& json);
    // given a list of requests, perform routing, and step once
    void navigate(std::vector<Request> const& requests);
    void navigate() { navigate({}); }
    Route get_route() const noexcept { return _curr_route; }

private:
    const int _map_x{0};  /// map size x
    const int _map_y{0};  /// map size y
    State _curr_state;    /// current vehicle state
    Route _curr_route;    /// current routing
    mutable std::unordered_set<State, State_hash, State_compare> _visited{};  /// history for search
    std::unordered_map<int, Request> _requests{};        /// all the request by id
    std::unordered_map<int, std::set<int>> _pickups{};   /// pickup at certain location
    std::unordered_map<int, std::set<int>> _dropoffs{};  /// dropoff at certain location
    // check out of bound
    inline bool is_valid(Coord const& coord) const noexcept
    {
        return coord.x >= 0 && coord.y >= 0 && coord.x < _map_x && coord.y < _map_y;
    }
    // covert from coord to index
    inline int coord_to_idx(Coord const& coord) const noexcept
    {
        return coord.y * _map_x + coord.x;
    }
    // try pickup at a location with given state
    void perform_pickup(int location_idx,
                        std::set<int>& passengers,
                        std::set<int>& fulfilled,
                        bool print = false) const noexcept;
    // try drop at a location with given state
    void perform_dropoff(int location_idx,
                         std::set<int>& passengers,
                         std::set<int>& fulfilled,
                         bool print = false) const noexcept;
    // BFS/Dijkstra
    Route BFS() const;
    // Astar
    Route Astar() const;
    // generate neighboring state for search algorithms
    std::vector<State> get_neighbors(State const& curr) const noexcept;
    // heuristic cost for Astar
    int h_cost(State const& state) const noexcept;
    // move state to new location, also perform actions
    void update_state(State& state, Coord const& coord, bool print = false) const noexcept;
    // cleanup finished requests
    void cleanup_request();
};

}  // namespace router
