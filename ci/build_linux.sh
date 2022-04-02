#!/bin/bash
set -e
set -o pipefail
set -x

cmake $(dirname $(realpath $0))/..
cmake --build . -j --target package
ctest