#!/bin/sh

cd pcf/build
cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ ..
make install

cd ../../experiment/build
cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ ..
make install

