find_package (Python COMPONENTS NumPy)
include_directories(.)

add_library(matplotlibcpp INTERFACE)
target_include_directories(matplotlibcpp INTERFACE . ${Python_INCLUDE_DIRS} ${Python_NumPy_INCLUDE_DIRS})
target_link_directories(matplotlibcpp INTERFACE ${Python_LIBRARIES})
# target_link_libraries(matplotlibcpp INTERFACE ${Python_LIBRARIES})
