param (
    [ValidateSet("x86", "x64")] $arch
)
. $PSScriptRoot\start_devenv.ps1
Start-Devenv $arch

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
$PSDefaultParameterValues['*:ErrorAction']='Stop'

cmake.exe $PSScriptRoot\.. 
cmake.exe --build . -j --target package
ctest.exe -C Debug --output-on-failure
