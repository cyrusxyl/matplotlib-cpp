#include <rapidjson/document.h>
#include <chrono>
#include <iostream>
#include "router.hpp"

int main(int /*argc*/, char** /*argv*/)
{
    std::string json =
        R"({"requests":[{"name":"Cyrus", "start":[0, 0], "end":[9, 9]}, {"name":"Jess", "start":[9, 9], "end":[2, 2]}]})";

    auto requests = router::RequestFactory::make_requests(json);

    router::Router router{10, 10, {0, 0}};
    // router::Request request1{0, "cyrus", {0, 0}, {9, 9}};
    // router::Request request2{1, "jess", {9, 9}, {2, 2}};
    router.navigate({requests});
    router.navigate({});
    router.navigate({});
    router.navigate({});
    // router.navigate({request2});
    router.navigate({});
    router.navigate({});
    return EXIT_SUCCESS;
}
