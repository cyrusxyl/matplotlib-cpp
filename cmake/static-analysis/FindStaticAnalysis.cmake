option(USE_CLANG_TIDY "Use clang-tidy for static analysis" ON)
if(USE_CLANG_TIDY)
    # Enable clang-tidy static analysis
    include(cmake/static-analysis/clang-tidy.cmake)
endif()

option(USE_CPPCHECK "Use cppcheck for satic analysis" OFF)
if(USE_CPPCHECK)
    # Enable cppcheck static analysis
    include(cmake/static-analysis/cppcheck.cmake)
endif()

default_build_type("Release")
