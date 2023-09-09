#!/usr/bin/bash

cmake -B build -S .
cmake --build build -j`nproc`

cp build/BoarCastle .
