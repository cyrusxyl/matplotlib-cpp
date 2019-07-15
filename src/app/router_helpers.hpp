// helper classes and functions
#pragma once
#include <rapidjson/document.h>
#include <array>
#include <boost/container_hash/hash.hpp>
#include <queue>
#include <set>
#include <vector>

namespace router
{
// coordinate with some helper
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

std::ostream& operator<<(std::ostream& out, Coord const& coord)
{
    out << '(' << coord.x << ',' << coord.y << ')';
    return out;
}
constexpr std::array<Coord, 4> MOVES{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
using Route = std::queue<Coord>;

// state of our vehicle
struct State
{
    Coord car_location;        // current locations
    std::set<int> passengers;  // set of passengers id, could be unordered but hashing is pain
    std::set<int> fulfilled;   // set of fulfilled request id
    Route route;               // route to here
};
// hashing function for state
struct State_hash
{
    std::size_t operator()(State const& state) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, state.car_location.x);
        boost::hash_combine(seed, state.car_location.y);
        boost::hash_combine(seed, state.passengers);
        boost::hash_combine(seed, state.fulfilled);
        // we don't care about route, if location and requests status is same, no need to visit
        // again
        return seed;
    }
};
struct State_compare
{
    bool operator()(State const& a, State const& b) const
    {
        return a.car_location.x == b.car_location.x && a.car_location.y == b.car_location.y &&
               a.passengers == b.passengers && a.fulfilled == b.fulfilled;
        // we don't care about route, if location and requests status is same, no need to
        // visit again
    }
};

struct Request
{
    int id{0};  // an id for better unique-ness. potentially need to care about overflowing
    std::string name;
    Coord pickup;
    Coord dropoff;
};
std::ostream& operator<<(std::ostream& out, Request const& request)
{
    out << request.name << " from " << request.pickup << " to " << request.dropoff;
    return out;
}

struct RequestFactory
{
    static int id;
    // make request from json
    static std::vector<Request> make_requests(std::string json)
    {
        std::vector<Request> requests;
        rapidjson::Document document;
        document.Parse(json.c_str());
        if (document.HasParseError())
        {
            throw std::invalid_argument("json ill defined");
        }
        if (!document.HasMember("requests") || !document["requests"].IsArray())
        {
            throw std::invalid_argument("need element \"requests\" to be an array");
        }
        auto const& json_requests = document["requests"];
        for (auto it = json_requests.Begin(); it != json_requests.End(); ++it)
        {
            if (!it->HasMember("name") || !it->HasMember("start") || !it->HasMember("end"))
            {
                throw std::invalid_argument("each reqest to have \"name\", \"start\", and \"end\"");
            }
            auto const& name = (*it)["name"];
            if (!name.IsString())
            {
                throw std::invalid_argument("\"name\" needs to be a string");
            }
            auto const& start = (*it)["start"];
            if (!start.IsArray() || start.Size() != 2)
            {
                throw std::invalid_argument("\"start\" needs to be a array of size 2 int");
            }
            auto const& end = (*it)["end"];
            if (!end.IsArray() || end.Size() != 2)
            {
                throw std::invalid_argument("\"end\" needs to be a array of size 2 int");
            }
            Request new_request;
            // increment id
            new_request.id = id++;
            new_request.name = name.GetString();
            new_request.pickup = {start[0].GetInt(), start[1].GetInt()};
            new_request.dropoff = {end[0].GetInt(), end[1].GetInt()};
            requests.push_back(new_request);
        }
        return requests;
    }
};
int RequestFactory::id = INT_MIN;

}  // namespace router
