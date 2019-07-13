add_library(router SHARED router.cpp)
target_compile_options(router PUBLIC -O3)

add_executable(myapp main.cpp)
target_link_libraries(myapp PUBLIC router)

#add_subdirectory(test)
