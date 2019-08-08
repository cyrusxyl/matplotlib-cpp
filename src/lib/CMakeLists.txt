find_package(Python COMPONENTS NumPy)

add_library(matplotlibcpp INTERFACE)
target_include_directories(matplotlibcpp INTERFACE include ${Python_INCLUDE_DIRS} ${Python_NumPy_INCLUDE_DIRS})
target_link_libraries(matplotlibcpp INTERFACE ${Python_LIBRARIES})
target_compile_options(matplotlibcpp INTERFACE "-w" "-O3")

install(TARGETS matplotlibcpp ${DEFAULT_DESTINATIONS})
install(DIRECTORY include DESTINATION .)