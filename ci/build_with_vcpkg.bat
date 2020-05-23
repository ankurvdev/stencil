if not defined DevEnvDir (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
)
if not defined BuildConfig (set BuildConfig="Debug")

git clone https://github.com/Microsoft/vcpkg.git
git -C vcpkg apply  --ignore-space-change --ignore-whitespace %~dp0vcpkg\vcpkg.stencil.patch
call vcpkg/bootstrap-vcpkg.bat
echo %~dp0..\ > vcpkg\ports\stencil\use_source_path
vcpkg\vcpkg install stencil
cmake.exe -G "Ninja" -DVCPKG_ROOT=vcpkg %~dp0\vcpkg
cmake.exe --build . -j --target package
ctest.exe