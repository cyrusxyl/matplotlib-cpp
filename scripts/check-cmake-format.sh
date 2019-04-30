#!/bin/bash

# Check if git is installed.
if ! type git > /dev/null; then
  echo \"Please install git.\"
  exit 1
fi

# Run the script to run cmake format in place
./scripts/run-cmake-format.sh

# Format checks may run in parallel on differnt file types.
# To make sure git diff reports only relevant changes,
# we pipe it through the same find call as above.
find $ROOT -iname CMakeLists.txt -o -iname *.cmake \
    | xargs git diff -- >> cmake_format.patch

# Return success if cmake-format made no changes
if [ ! -s cmake_format.patch ]
then
    echo "CMake files are formatted properly."
    rm cmake_format.patch
    exit 0
fi

# Fail if any changes were made
echo "Please, format your cmake files before submitting!"
echo "---------------------------------------------------"
echo
echo "Run cmake-format on the following files:"
echo
git ls-files -m '*/CMakeLists.txt' '*.cmake'
echo
echo "==================================================="
echo
echo "The following changes were made by cmake-format:"
echo
cat cmake_format.patch
echo
echo "Run ./scripts/run-cmake-format.sh to fix issues."
echo

exit 1