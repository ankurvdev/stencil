param (
    [ValidateSet("x86", "x64")] $arch
)
. $PSScriptRoot\start_devenv.ps1
Start-Devenv $arch

$ErrorActionPreference = "Stop"

cmake.exe $PSScriptRoot\..
cmake.exe --build . -j --target package
ctest.exe -C Debug
