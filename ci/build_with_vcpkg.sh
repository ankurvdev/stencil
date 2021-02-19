scriptdir=$(dirname $(realpath $0))
git clone -q https://github.com/ankurverma85/vcpkg.git --branch ankurv/stencil --depth 1
rm -rf vcpkg/ports/stencil
cp -r ${scriptdir}/vcpkg/port.stencil vcpkg/ports/stencil
echo ${scriptdir}/.. > vcpkg/ports/stencil/use_source_path
vcpkg/bootstrap-vcpkg.sh
vcpkg/vcpkg install stencil
mkdir Test-Debug
pushd Test-Debug
cmake -DCMAKE_BUILD_TYPE=Debug -DVCPKG_ROOT=$(realpath ./vcpkg) -DVCPKG_VERBOSE:BOOL=ON $scriptdir\vcpkg
cmake --build . -j --target package
ctest .
mkdir Test-Release
pushd Test-Release
cmake -DCMAKE_BUILD_TYPE=Release -DVCPKG_ROOT=$(realpath ./vcpkg) -DVCPKG_VERBOSE:BOOL=ON $scriptdir\vcpkg
cmake --build . -j --target package
ctest .
