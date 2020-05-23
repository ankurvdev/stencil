scriptdir=$(dirname $(realpath $0))
git clone https://github.com/Microsoft/vcpkg.git
git -C vcpkg apply --ignore-space-change --ignore-whitespace $scriptdir/vcpkg/vcpkg.stencil.patch
echo ${scriptdir}/.. > vcpkg/ports/stencil/use_source_path
vcpkg/bootstrap-vcpkg.sh
vcpkg/vcpkg install stencil
cmake.exe -G "Ninja" -DVCPKG_ROOT=vcpkg %~dp0\vcpkg
cmake.exe --build . -j --target package
ctest.exe