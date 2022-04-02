#!/bin/bash
set -e
set -o pipefail
set -x

cmake -DCMAKE_CXX_FLAGS=-stdlib=libstdc++-11 $(dirname $(realpath $0))/..
cmake --build . -j --target package
ctest