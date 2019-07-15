#include <gtest/gtest.h>
#include <app/router.hpp>

TEST(RequestTest, Invalid)
{
    // ill defined
    EXPECT_THROW(router::RequestFactory::make_requests(""), std::invalid_argument);
    // empty
    EXPECT_THROW(router::RequestFactory::make_requests("{}"), std::invalid_argument);
    // requests is not an array
    EXPECT_THROW(router::RequestFactory::make_requests(R"({"requests": 1})"),
                 std::invalid_argument);
    // request element does not contain information
    EXPECT_THROW(router::RequestFactory::make_requests(R"({"requests": [{}]})"),
                 std::invalid_argument);
    // tag incorrect
    EXPECT_THROW(router::RequestFactory::make_requests(
                     R"({"requests":[{"notaname":"Cyrus", "start":[0, 1], "end":[3, 3]})"),
                 std::invalid_argument);
    // coordinate not correct
    EXPECT_THROW(router::RequestFactory::make_requests(
                     R"({"requests":[{"name":"Cyrus", "start":[0, 1, 1], "end":[3, 3]})"),
                 std::invalid_argument);
}

TEST(RequestTest, Empty)
{
    // empty requests array
    auto requests = router::RequestFactory::make_requests(R"({"requests": []})");
    EXPECT_EQ(requests.size(), 0);
}

TEST(RequestTest, Trivial)
{
    auto requests1 = router::RequestFactory::make_requests(
        R"({"requests":[{"name":"Cyrus", "start":[0, 1], "end":[3, 3]}]})");
    EXPECT_EQ(requests1.size(), 1);
    auto requests2 = router::RequestFactory::make_requests(
        R"({"requests":[{"name":"Cyrus", "start":[0, 1], "end":[3, 4]}, {"name":"Jess", "start":[5, 6], "end":[7, 8]}]})");
    EXPECT_EQ(requests2.size(), 2);
    EXPECT_EQ(requests2[0].name, "Cyrus");
    EXPECT_EQ(requests2[0].pickup.x, 0);
    EXPECT_EQ(requests2[0].pickup.y, 1);
    EXPECT_EQ(requests2[0].dropoff.x, 3);
    EXPECT_EQ(requests2[0].dropoff.y, 4);
    EXPECT_EQ(requests2[1].name, "Jess");
    EXPECT_EQ(requests2[1].pickup.x, 5);
    EXPECT_EQ(requests2[1].pickup.y, 6);
    EXPECT_EQ(requests2[1].dropoff.x, 7);
    EXPECT_EQ(requests2[1].dropoff.y, 8);
    EXPECT_NE(requests1[0].id, requests2[0].id);
    EXPECT_NE(requests1[0].id, requests2[1].id);
    EXPECT_NE(requests2[0].id, requests2[1].id);
}

TEST(StateTest, Hash)
{
    std::queue<router::Coord> q;
    q.push({0, 0});
    q.push({0, 1});
    router::State state1 = {{2, 3}, {4, 5, 6}, {1, 2, 3}, {}};
    router::State state2 = {{2, 3}, {6, 5, 4}, {2, 3, 1}, {}};
    router::State state3 = {{2, 3}, {4, 5, 6}, {1, 2, 3}, q};
    router::State state4 = {{3, 2}, {4, 5, 6}, {1, 2, 3}, {}};
    router::State state5 = {{2, 3}, {4, 5}, {1, 2, 3}, {}};
    router::State state6 = {{2, 3}, {4, 5, 6}, {}, {}};
    router::State_hash hashing;
    // same coordinate, same content, order doesn't matter, should not visit again
    EXPECT_EQ(hashing(state1), hashing(state2));
    // same coordinate, same content, route doesn't matter, should not visit again
    EXPECT_EQ(hashing(state1), hashing(state3));
    // different coordinate, same content, should visit
    EXPECT_NE(hashing(state1), hashing(state4));
    // different content, same coordinate, should visit
    EXPECT_NE(hashing(state1), hashing(state5));
    EXPECT_NE(hashing(state1), hashing(state6));
}

TEST(StateTest, Equal)
{
    std::queue<router::Coord> q;
    q.push({0, 0});
    q.push({0, 1});
    router::State state1 = {{2, 3}, {4, 5, 6}, {1, 2, 3}, {}};
    router::State state2 = {{2, 3}, {6, 5, 4}, {2, 3, 1}, {}};
    router::State state3 = {{2, 3}, {4, 5, 6}, {1, 2, 3}, q};
    router::State state4 = {{3, 2}, {4, 5, 6}, {1, 2, 3}, {}};
    router::State state5 = {{2, 3}, {4, 5}, {1, 2, 3}, {}};
    router::State state6 = {{2, 3}, {4, 5, 6}, {}, {}};
    router::State_compare compare;
    // order doesn't matter, should not visit again
    EXPECT_TRUE(compare(state1, state2));
    // route doesn't matter, should not visit again
    EXPECT_TRUE(compare(state1, state3));
    // different coordinate, same content, should visit
    EXPECT_FALSE(compare(state1, state4));
    // different content, same coordinate, should visit
    EXPECT_FALSE(compare(state1, state5));
    EXPECT_FALSE(compare(state1, state6));
}

TEST(RouterTest, Trivial)
{
    std::string json = R"({"requests":[{"name":"Cyrus", "start":[0, 1], "end":[3, 3]}]})";
    router::Router router{10, 10, {0, 0}};
    router.navigate_json(json);
    EXPECT_EQ(router.get_route().size(), 6);
}

TEST(RouterTest, Stepping)
{
    std::string json = R"({"requests":[{"name":"Cyrus", "start":[0, 1], "end":[3, 3]}]})";
    router::Router router{10, 10, {0, 0}};
    router.navigate_json(json);
    EXPECT_EQ(router.get_route().size(), 6);
    router.navigate({});
    EXPECT_EQ(router.get_route().size(), 5);
    router.navigate();
    EXPECT_EQ(router.get_route().size(), 4);
}

TEST(RouterTest, Multi)
{
    std::string json1 =
        R"({"requests":[{"name":"Cyrus", "start":[0, 1], "end":[3, 3]}, {"name":"Jess", "start":[0, 4], "end":[3, 3]}]})";
    std::string json2 = R"({"requests": []})";
    std::string json3 = R"({"requests":[{"name":"Roger", "start":[1, 3], "end":[9, 9]}]})";

    router::Router router{10, 10, {0, 0}};
    router.navigate_json(json1);
    EXPECT_EQ(router.get_route().size(), 8);
    router.navigate_json(json2);
    EXPECT_EQ(router.get_route().size(), 7);
    router.navigate_json(json3);
    EXPECT_EQ(router.get_route().size(), 19);
}

TEST(RouterTest, Long)
{
    std::string json1 =
        R"({"requests":[{"name":"Cyrus", "start":[0, 1], "end":[9, 9]}, {"name":"Jess", "start":[9, 0], "end":[0, 9]}]})";

    router::Router router{10, 10, {0, 0}};
    router.navigate_json(json1);
    EXPECT_EQ(router.get_route().size(), 29);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
