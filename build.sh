#!/bin/bash

# Main build script

# if not installed
#sudo apt update
#sudo apt install cmake coreutils gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential ninja-build

# build proccess
mkdir -p build
cd build
cmake -G Ninja ..
ninja
cd ..
# done