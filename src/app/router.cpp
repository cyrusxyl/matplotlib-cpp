#include "router.hpp"

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
        return;
    }
}

}  // namespace router
