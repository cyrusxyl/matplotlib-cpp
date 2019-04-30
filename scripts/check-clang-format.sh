#!/usr/bin/env bash
# This script should be run from the project root.

# Check if git is installed.
if ! type git > /dev/null; then
  echo \"Please install git.\"
  exit 1
fi

# Run the script to run clang format in place
./scripts/run-clang-format.sh

git diff > clang_format.patch

if [ ! -s clang_format.patch ]
then
    echo \"Code is formatted properly.\"
    rm clang_format.patch
    exit 0
fi

echo "==================================================="
echo
echo "The following changes were made by clang-format:"
cat clang_format.patch
echo
echo "==================================================="
echo
echo "Please, run clang-format on the following files:"
git ls-files -m
echo
echo "Run ./scripts/run-clang-format.sh to fix issues."
echo

exit 1