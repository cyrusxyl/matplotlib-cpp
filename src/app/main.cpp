#include <chrono>
#include <iostream>
#include "router.hpp"

int main(int /*argc*/, char** /*argv*/)
{
    router::Router router{10, 10, {0, 0}};
    router::Request request1{0, "cyrus", {0, 1}, {5, 5}};
    router::Request request2{1, "jess", {0, 2}, {5, 5}};
    auto t1 = std::chrono::system_clock::now();
    router.navigate({request1, request2});
    auto t2 = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count() << '\n';
    return EXIT_SUCCESS;
}
