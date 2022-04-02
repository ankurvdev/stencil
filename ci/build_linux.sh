#!/bin/bash
set -e
set -o pipefail
set -x

cmake -DCMAKE_CXX_FLAGS=-stdlib=libc++ $(dirname $(realpath $0))/..
cmake --build . -j --target package
ctest