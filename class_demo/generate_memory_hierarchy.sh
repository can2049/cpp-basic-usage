#!/bin/bash

# g++ -fdump-lang-class ./

DUMP_CLASS_OPTION=""
# if gcc version <= 7, set option to -fdump-class-hierarchy
# if gcc version >= 8, set option to -fdump-lang-class
GCC_VERSION=$(g++ --version | grep -oP '\d+\.\d+' | head -n 1)
if [[ $(echo "$GCC_VERSION < 8" | bc) -eq 1 ]]; then
    DUMP_CLASS_OPTION="-fdump-class-hierarchy"
else
    DUMP_CLASS_OPTION="-fdump-lang-class"
fi

# get c++ files into list in current directory
CXX_FILES=$(find . -name "*.cpp" -o -name "*.cc")

# for every cxx file, generate class hierarchy
for file in $CXX_FILES; do
    # generate class hierarchy
    g++ $DUMP_CLASS_OPTION "$file"
done
