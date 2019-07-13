#pragma once
#include <array>
#include <optional>
#include <vector>

namespace router
{
struct Coord
{
    int x{0};
    int y{0};
    Coord move(Coord const& direction)
    {
        int nx = x + direction.x;
        int ny = y + direction.y;
        return {nx, ny};
    }
    int distance(Coord const& other)
    {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }
};
constexpr std::array<Coord, 4> MOVES{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

struct State
{
    Coord car_location;
    std::vector<int> passenges;
    std::vector<int> pickups;
    std::vector<int> dropoffs;
};
using Route = std::vector<State>;

struct Request
{
    static std::size_t idgen;
    const std::size_t id = idgen++;
    std::string name{""};
    Coord pickup;
    Coord dropoff;
};
std::size_t Request::idgen{0};
}  // namespace router