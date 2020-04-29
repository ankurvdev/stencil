git clone https://github.com/Microsoft/vcpkg.git
set VCPKG_USE_STENCIL_SRC_DIR="%~dp0\.."
git -C vcpkg apply  --ignore-space-change --ignore-whitespace %~dp0vcpkg\vcpkg.stencil.patch
call vcpkg/bootstrap-vcpkg.bat
vcpkg\vcpkg install stencil