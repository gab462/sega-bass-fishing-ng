#!/bin/sh

[ ! -d ./DPP/build/ ] &&
    pushd ./DPP/ &&
    cmake -B ./build -DCMAKE_INSTALL_PREFIX=$PWD/build/install &&
    cmake --build ./build -j16 &&
    pushd build &&
    make install &&
    popd && popd

c++ -std=c++23 -Wall -Wextra -I ./DPP/build/install/include/ main.cpp -o main -L ./DPP/build/install/lib/ -ldpp -Wl,-rpath ./DPP/build/install/lib/
