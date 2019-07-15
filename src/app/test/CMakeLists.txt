add_executable(router_test router_test.cpp)
target_link_libraries(router_test PUBLIC router CONAN_PKG::gtest)
