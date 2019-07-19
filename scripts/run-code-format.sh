#!/usr/bin/env bash
# This script should be run from the project root.

# Check if clang-format is installed.
if ! type clang-format-6.0 > /dev/null; then
  echo \"Please install clang-format.\"
  exit 1
fi

find ./src -iname *.h -o -iname *.hpp -o -iname *.cpp -o -iname *.c -o -iname *.cc -o -iname *.tpp \
    | xargs clang-format-6.0 -style=file -i -fallback-style=none
    
# Check if cmake-format is installed.
if ! type cmake-format > /dev/null; then
  echo \"Please install cmake-format.\"
  exit 1
fi


# Run cmake-format an all CMakeLists.txt and *.cmake files
ROOT=$(git rev-parse --show-toplevel)
find $ROOT -iname CMakeLists.txt -o -iname *.cmake \
    | xargs cmake-format -i