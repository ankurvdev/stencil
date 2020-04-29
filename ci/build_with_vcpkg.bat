git clone https://github.com/Microsoft/vcpkg.git
git -C vcpkg apply  --ignore-space-change --ignore-whitespace %~dp0vcpkg\vcpkg.stencil.patch
call vcpkg/bootstrap-vcpkg.bat
echo %~dp0..\ > vcpkg\ports\stencil\use_source_path
vcpkg\vcpkg install stencil