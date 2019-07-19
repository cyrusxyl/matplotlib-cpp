find_program(CPPCHECK_COMMAND NAMES cppcheck DOC "Path to CppCheck executable")
if(NOT CPPCHECK_COMMAND)
    message(FATAL_ERROR "CppCheck executable is not found.")
else()
    message(STATUS "CppCheck: found ${CPPCHECK_COMMAND}")
    list(APPEND CPPCHECK_COMMAND
                "--enable=warning,performance,portability"
                "--inconclusive"
                "--inline-suppr")
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK_COMMAND})
endif()
