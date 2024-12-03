#!/bin/sh

set -xe

[ ! -d ./DPP/build/ ] &&
    pushd ./DPP/ &&
    cmake -B ./build -DCMAKE_INSTALL_PREFIX=$PWD/build/install &&
    cmake --build ./build -j16 &&
    pushd build &&
    make install &&
    popd && popd

[ ! -f ./libolive.a ] &&
    cc -std=c99 -DOLIVEC_IMPLEMENTATION -DOLIVECDEF="" -c ./olive.c -o ./olive.o &&
    ar rcs ./libolive.a ./olive.o

[ ! -f ./libmsf_gif.a ] &&
    cc -std=c99 -x c -DMSF_GIF_IMPL -c ./msf_gif.h -o ./msf_gif.o &&
    ar rcs ./libmsf_gif.a ./msf_gif.o

c++ -std=c++23 -Wall -Wextra -I ./DPP/build/install/include/ main.cpp -o main -L . -l:libolive.a -l:libmsf_gif.a -L ./DPP/build/install/lib/ -ldpp -Wl,-rpath ./DPP/build/install/lib/ -ggdb
