#include <chrono>
#include <iostream>
#include "router.hpp"

int main(int /*argc*/, char** /*argv*/)
{
    router::Router router{10, 10, {0, 0}};
    router::Request request1{0, "cyrus", {0, 0}, {9, 9}};
    router::Request request2{1, "jess", {9, 9}, {2, 2}};
    router.navigate({request1});
    router.navigate({});
    router.navigate({});
    router.navigate({});
    router.navigate({request2});
    router.navigate({});
    router.navigate({});
    return EXIT_SUCCESS;
}
