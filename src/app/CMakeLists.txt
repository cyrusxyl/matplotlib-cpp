add_executable(myapp main.cpp)

target_link_libraries(myapp PUBLIC tempClass)

add_library(tempClass STATIC TempClass.cpp)

add_subdirectory(test)
