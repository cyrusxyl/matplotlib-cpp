#include "router.hpp"

#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
    router::Router router{10, 10, {0, 0}};
    router::Request request1{0, "cyrus", {0, 1}, {5, 5}};
    router::Request request2{1, "jess", {5, 6}, {0, 1}};

    router.navigate({request1, request2});
    return EXIT_SUCCESS;
}
