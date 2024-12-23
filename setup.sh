#!/bin/bash

mkdir -p build
cd build
cmake ..
make
mv main ../rubix
cd ..
rm -r build
