#pragma once
#include <array>
#include <boost/container_hash/hash.hpp>
#include <optional>
#include <queue>
#include <set>
#include <vector>

namespace router
{
struct Coord
{
    int x{0};
    int y{0};
    Coord move(Coord const& direction) const noexcept
    {
        int nx = x + direction.x;
        int ny = y + direction.y;
        return {nx, ny};
    }
    int distance(Coord const& other) const noexcept
    {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }
};
constexpr std::array<Coord, 4> MOVES{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
using Route = std::queue<Coord>;

struct State
{
    Coord car_location;
    std::set<int> passenges;
    std::set<int> fullfilled;
    Route route;
};
struct State_hash
{
    std::size_t operator()(State const& state) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, state.car_location.x);
        boost::hash_combine(seed, state.car_location.y);
        boost::hash_combine(seed, state.passenges);
        boost::hash_combine(seed, state.fullfilled);
        return seed;
    }
};
struct State_compare
{
    bool operator()(State const& a, State const& b) const
    {
        return a.car_location.x == b.car_location.x &&
               a.car_location.y == b.car_location.y &&
               a.passenges == b.passenges && a.fullfilled == b.fullfilled;
    }
};

struct Request
{
    int id{0};
    std::string name;
    Coord pickup;
    Coord dropoff;
};

}  // namespace router
