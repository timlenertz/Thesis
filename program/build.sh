#!/bin/sh

mkdir -p pcf/build &&
cd pcf/build &&
cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ .. &&
make install
