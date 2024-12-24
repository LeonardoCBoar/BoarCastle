#!/usr/bin/bash

export CMAKE_MAKE_PROGRAM=make && cmake -B build -S .
export CMAKE_MAKE_PROGRAM=make && cmake --build build -j`nproc`

cp build/BoarCastle .
