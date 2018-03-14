#!/bin/bash
#if [ -d build ]; then
#    rm -r build;
#fi

if [ ! -d build ]; then
#    rm -r build;
    mkdir build;
fi

cd build;
cmake ..;
make -j4;

cd ..;


