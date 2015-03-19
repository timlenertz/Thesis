#!/bin/sh

mkdir -p experiment/build &&
cd experiment/build &&
cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ .. &&
make install
