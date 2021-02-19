param (
    [ValidateSet("x86", "x64")] $arch
)
. $PSScriptRoot\start_devenv.ps1
Start-Devenv $arch
$ErrorActionPreference = "Stop"
Invoke-Expression ("python $PSScriptRoot\build_with_vcpkg.py --triplet " + ($arch + "-windows"))
