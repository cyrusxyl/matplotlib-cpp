#!/bin/bash

# Check if cmake-format is installed.
if ! type cmake-format > /dev/null; then
  echo \"Please install cmake-format.\"
  exit 1
fi


# Run cmake-format an all CMakeLists.txt and *.cmake files
ROOT=$(git rev-parse --show-toplevel)
find $ROOT -iname CMakeLists.txt -o -iname *.cmake \
    | xargs cmake-format -i
