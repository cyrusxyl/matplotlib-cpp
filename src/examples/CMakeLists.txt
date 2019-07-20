add_executable(minimal minimal.cpp)
target_link_libraries(minimal matplotlibcpp)

add_executable(basic basic.cpp)
target_link_libraries(basic matplotlibcpp)

add_executable(modern modern.cpp)
target_link_libraries(modern matplotlibcpp)

add_executable(animation animation.cpp)
target_link_libraries(animation matplotlibcpp)

add_executable(nonblock nonblock.cpp)
target_link_libraries(nonblock matplotlibcpp)

add_executable(xkcd xkcd.cpp)
target_link_libraries(xkcd matplotlibcpp)

add_executable(bar bar.cpp)
target_link_libraries(bar matplotlibcpp)