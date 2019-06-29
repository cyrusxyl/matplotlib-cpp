#include "TempClass.hpp"

#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
    TempClass testObject;

    std::cout << "Test object data : " << testObject.getData() << std::endl;

    testObject.setData(12);
    std::cout << "Test object data : " << testObject.getData() << std::endl;

    return EXIT_SUCCESS;
}
