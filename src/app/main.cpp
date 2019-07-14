#include <rapidjson/document.h>
#include <chrono>
#include <iostream>
#include "router.hpp"

int main(int /*argc*/, char** /*argv*/)
{
    std::string json_1 =
        R"({"requests":[{"name":"Cyrus", "start":[0, 1], "end":[3, 3]}, {"name":"Jess", "start":[0, 2], "end":[2, 2]}]})";

    router::Router router{10, 10, {0, 0}};
    router.navigate_json(json_1);
    router.navigate_json(R"({"requests":[]})");
    router.navigate_json(R"({"requests":[]})");
    router.navigate_json("");
    router.navigate_json(R"({"requests":[{"name":"Novak", "start":[2, 1], "end":[3, 3]}]})");
    
    return EXIT_SUCCESS;
}
