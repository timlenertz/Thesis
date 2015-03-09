#!/bin/sh

mkdir pcf/build
cd pcf/build
cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ ..
make install

cd ../../
mkdir experiment/build
cd experiment/build
cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ ..
make install

