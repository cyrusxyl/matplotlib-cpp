#include <gtest/gtest.h>

#include <thread>

// LSan, ASan
TEST(Sanitizer, memory_leak)
{
    auto* buffer = new char[16];
    sprintf(buffer, "I'm leaking!");
    printf("%s\n", buffer);
}

// ASan, TSan
TEST(Sanitizer, use_after_free)
{
    auto* array = new int[16];
    delete[] array;
    std::cout << array[0] << std::endl;  // BOOM
}

// UBSan
TEST(Sanitizer, undefined_behavior)
{
    int k = 0x7fffffff;
    k += 1;
}

TEST(Sanitizer, data_race)
{
    int data;

    std::thread t([&] { data = 1; });
    data = 2;
    t.join();
}