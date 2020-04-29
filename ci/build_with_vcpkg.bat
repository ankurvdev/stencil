git clone https://github.com/Microsoft/vcpkg.git
set VCPKG_USE_STENCIL_SRC_DIR="%~dp0\.."
git apply --ignore-space-change --ignore-whitespace %~dp0/vcpkg/*patch
vcpkg/bootstrap-vcpkg.bat
vcpkg/vcpkg install stencil:x86-windows
