project_add_test(tempClassTest RunAllTests.cpp TempClassTest.cpp)

target_link_libraries(tempClassTest tempClass CONAN_PKG::gtest)
