# http://mariobadr.com/using-clang-tidy-with-cmake-36.html
find_program(CLANG_TIDY_COMMAND NAMES clang-tidy clang-tidy-6.0 DOC "Path to clang-tidy executable")
if(NOT CLANG_TIDY_COMMAND)
    message(FATAL_ERROR "clang-tidy executable is not found.")
else()
    message(STATUS "Clang-tidy: found ${CLANG_TIDY_COMMAND}")
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_COMMAND})
endif()

macro(disable_clang_tidy __target)
    set_property(TARGET ${__target} PROPERTY CXX_CLANG_TIDY "")
endmacro()
