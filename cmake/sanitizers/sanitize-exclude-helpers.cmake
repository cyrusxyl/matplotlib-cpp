define_property(TARGET
                PROPERTY EXCLUDE_FROM_SANITIZE_ALL
                BRIEF_DOCS "Exclude target from all sanitizers"
                FULL_DOCS "Do not apply any code sanitizers to the target.")
define_property(TARGET
                PROPERTY EXCLUDE_FROM_SANITIZE_ADDRESS
                BRIEF_DOCS "Exclude target for ASan"
                FULL_DOCS "Do not apply Address Sanitizer to the target.")
define_property(TARGET
                PROPERTY EXCLUDE_FROM_SANITIZE_LEAK
                BRIEF_DOCS "Exclude target for LSan"
                FULL_DOCS "Do not apply Leak Sanitizer to the target.")
define_property(TARGET
                PROPERTY EXCLUDE_FROM_SANITIZE_MEMORY
                BRIEF_DOCS "Exclude target for MSan"
                FULL_DOCS "Do not apply Memory Sanitizer to the target.")
define_property(TARGET
                PROPERTY EXCLUDE_FROM_SANITIZE_THREAD
                BRIEF_DOCS "Exclude target for TSan"
                FULL_DOCS "Do not apply Thread Sanitizer to the target.")
define_property(TARGET
                PROPERTY EXCLUDE_FROM_SANITIZE_UNDEFINED
                BRIEF_DOCS "Exclude target for UBSan"
                FULL_DOCS "Do not apply Undefined Behavior Sanitizer to the target.")

set(EXCLUDE_FROM_SANITIZE_OPTIONS
    EXCLUDE_FROM_SANITIZE_ALL
    EXCLUDE_FROM_SANITIZE_ADDRESS
    EXCLUDE_FROM_SANITIZE_LEAK
    EXCLUDE_FROM_SANITIZE_MEMORY
    EXCLUDE_FROM_SANITIZE_THREAD
    EXCLUDE_FROM_SANITIZE_UNDEFINED)

function(set_target_sanitize_properties
         TARGET
         EXCLUDE_FROM_SANITIZE_ALL
         EXCLUDE_FROM_SANITIZE_ADDRESS
         EXCLUDE_FROM_SANITIZE_LEAK
         EXCLUDE_FROM_SANITIZE_MEMORY
         EXCLUDE_FROM_SANITIZE_THREAD
         EXCLUDE_FROM_SANITIZE_UNDEFINED)

    if(EXCLUDE_FROM_SANITIZE_ALL)
        set_property(TARGET ${TARGET} PROPERTY "EXCLUDE_FROM_SANITIZE_ALL" TRUE)
    endif()

    if(EXCLUDE_FROM_SANITIZE_ADDRESS)
        set_property(TARGET ${TARGET} PROPERTY "EXCLUDE_FROM_SANITIZE_ADDRESS" TRUE)
    endif()

    if(EXCLUDE_FROM_SANITIZE_LEAK)
        set_property(TARGET ${TARGET} PROPERTY "EXCLUDE_FROM_SANITIZE_LEAK" TRUE)
    endif()

    if(EXCLUDE_FROM_SANITIZE_MEMORY)
        set_property(TARGET ${TARGET} PROPERTY "EXCLUDE_FROM_SANITIZE_MEMORY" TRUE)
    endif()

    if(EXCLUDE_FROM_SANITIZE_THREAD)
        set_property(TARGET ${TARGET} PROPERTY "EXCLUDE_FROM_SANITIZE_THREAD" TRUE)
    endif()

    if(EXCLUDE_FROM_SANITIZE_UNDEFINED)
        set_property(TARGET ${TARGET} PROPERTY "EXCLUDE_FROM_SANITIZE_UNDEFINED" TRUE)
    endif()
endfunction()

function(get_target_exclude_sanitize_property EXCLUDE_SANITIZE TARGET PROPERTY_NAME)
    get_property(EXCLUDE_PROPERTY TARGET ${TARGET} PROPERTY "${PROPERTY_NAME}" SET)
    get_property(EXCLUDE_ALL TARGET ${TARGET} PROPERTY "EXCLUDE_FROM_SANITIZE_ALL" SET)
    if(EXCLUDE_PROPERTY OR EXCLUDE_ALL)
        set(EXCLUDE_SANITIZE TRUE PARENT_SCOPE)
    else()
        set(EXCLUDE_SANITIZE FALSE PARENT_SCOPE)
    endif()
endfunction()

# `add_library` and `add_executable` functions are redefined to support
# addtional target properties.  Supported unary options are provided in the
# EXCLUDE_FROM_SANITIZE_OPTIONS variable defined above.  If specified, they are
# pushed as properties of the target, which are respected by add_sanitizer_xxxx
# functions.
function(add_library TARGET)
    cmake_parse_arguments("" "${EXCLUDE_FROM_SANITIZE_OPTIONS}" "" "" ${ARGN})
    _add_library(${TARGET} ${_UNPARSED_ARGUMENTS})

    set_target_sanitize_properties(${TARGET}
                                   "${_EXCLUDE_FROM_SANITIZE_ALL}"
                                   "${_EXCLUDE_FROM_SANITIZE_ADDRESS}"
                                   "${_EXCLUDE_FROM_SANITIZE_LEAK}"
                                   "${_EXCLUDE_FROM_SANITIZE_MEMORY}"
                                   "${_EXCLUDE_FROM_SANITIZE_THREAD}"
                                   "${_EXCLUDE_FROM_SANITIZE_UNDEFINED}")

    if(SANITIZE_ALL_TARGETS)
        add_sanitizers(${TARGET})
    endif()
endfunction()

function(add_executable TARGET)
    cmake_parse_arguments("" "${EXCLUDE_FROM_SANITIZE_OPTIONS}" "" "" ${ARGN})
    _add_executable(${TARGET} ${_UNPARSED_ARGUMENTS})

    set_target_sanitize_properties(${TARGET}
                                   "${_EXCLUDE_FROM_SANITIZE_ALL}"
                                   "${_EXCLUDE_FROM_SANITIZE_ADDRESS}"
                                   "${_EXCLUDE_FROM_SANITIZE_LEAK}"
                                   "${_EXCLUDE_FROM_SANITIZE_MEMORY}"
                                   "${_EXCLUDE_FROM_SANITIZE_THREAD}"
                                   "${_EXCLUDE_FROM_SANITIZE_UNDEFINED}")

    if(SANITIZE_ALL_TARGETS)
        add_sanitizers(${TARGET})
    endif()
endfunction()
