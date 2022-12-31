#!/usr/bin/bash

git clone --depth=1 https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh --disableMetrics

