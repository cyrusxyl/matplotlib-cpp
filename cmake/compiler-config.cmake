set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_NO_CYCLES 1)

# Reference: https://github.com/lefticus/cppbestpractices/blob/master/02-Use_the
# _Tools_Available.md#gcc--clang GCC all versions, Clang all versions
add_compile_options(-Wall
                    -Wextra
                    -Wshadow
                    -Wnon-virtual-dtor
                    -Wold-style-cast
                    -Wcast-align
                    -Wunused
                    -Woverloaded-virtual
                    -Wconversion
                    -Werror)

# GCC all versions, Clang >= 3.2
if((CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   OR ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 3.2)))
    add_compile_options(-pedantic -Wpedantic)
endif()

# GCC >= 4.3, Clang all versions
if((CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
   OR ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 4.3)))
    add_compile_options(-Wsign-conversion)
endif()

# GCC >= 4.6, Clang >= 3.8
if(((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 4.6))
   OR ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 3.8)))
    add_compile_options(-Wdouble-promotion)
endif()

# Add fortify source if we have specified to enable it. This is an option so we
# can check it against sanitizers which it conflicts with. Supported in both gcc
# and clang.
option(COMPILER_FORTIFY_SOURCE "Enable the fortify source option" OFF)
if(COMPILER_FORTIFY_SOURCE)
    add_compile_options(-D_FORTIFY_SOURCE=2)
endif()
