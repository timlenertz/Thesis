#!/bin/sh

mkdir -p pcf/build &&
cd pcf/build &&
cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ .. &&
make install &&

cd ../../ &&
mkdir -p experiment/build &&
cd experiment/build &&
cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ .. &&
make install

