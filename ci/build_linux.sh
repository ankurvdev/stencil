cmake $(dirname $(realpath $0))/..
cmake --build . -j --target package
ctest