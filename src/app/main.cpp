#include "router.hpp"

#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
    router::Request a;
    router::Request b;
    std::cout << a.id << '\n';
    std::cout << b.id << '\n';
    return EXIT_SUCCESS;
}
