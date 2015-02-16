#!/bin/sh

cd build
cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ ..
make install
