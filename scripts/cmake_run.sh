#!/bin/sh
rm CMakeCache.txt
rm -r build
rm -r CMakeFiles
cmake .
clear
cmake --build . --target $1 

