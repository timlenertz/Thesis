#!/bin/sh

cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-OSX.txt ..
make
make install
