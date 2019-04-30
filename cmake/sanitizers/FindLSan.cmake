# The MIT License (MIT)
#
# Copyright (c) 2013 Matthew Arsenault 2015-2016 RWTH Aachen University, Federal
# Republic of Germany
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

option(SANITIZE_LEAK "Enable LeakSanitizer for sanitized targets." Off)

# cmake-format: off
set(FLAG_CANDIDATES
    # Clang 3.2+ use this version. The no-omit-frame-pointer option is optional.
    "-g -fsanitize=leak -fno-omit-frame-pointer"
    "-g -fsanitize=leak"
)
# cmake-format: on

if(SANITIZE_LEAK AND (SANITIZE_THREAD OR SANITIZE_MEMORY))
    message(FATAL_ERROR "LeakSanitizer is not compatible with " "ThreadSanitizer or MemorySanitizer.")
endif()

include(sanitize-helpers)

if(SANITIZE_LEAK)
    sanitizer_check_compiler_flags("${FLAG_CANDIDATES}" "LeakSanitizer" "LSan")
endif()

function(add_sanitize_leak TARGET)
    if(NOT SANITIZE_LEAK)
        return()
    endif()

    sanitizer_add_flags(${TARGET} "LeakSanitizer" "LSan")

    get_target_exclude_sanitize_property(EXCLUDE_SANITIZE ${TARGET} EXCLUDE_FROM_SANITIZE_LEAK)

    if(${EXCLUDE_SANITIZE})
        return()
    endif()

    sanitizer_add_flags(${TARGET} "LeakSanitizer" "LSan")
endfunction()
