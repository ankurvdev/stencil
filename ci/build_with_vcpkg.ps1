param (
    [ValidateSet("x86", "x64")] $arch
)
. $PSScriptRoot\start_devenv.ps1
Start-Devenv $arch
$ErrorActionPreference = "Stop"
if (! (Test-Path vcpkg)) {
    git clone -q https://github.com/ankurverma85/vcpkg.git --branch ankurv/stencil --depth 1
}

if ((Test-Path vcpkg\ports\stencil)) {
    Get-Item vcpkg\ports\stencil | Remove-Item -Recurse 
}

Copy-Item -Recurse $PSScriptRoot\vcpkg\port.stencil vcpkg\ports\stencil
Get-Item vcpkg\ports\stencil

$triplet = $arch + "-windows"
Set-Item ENV:/VCPKG_ROOT $(Resolve-Path vcpkg)
Set-Item ENV:/VCPKG_BINARY_SOURCES "clear"
if (! (Test-Path vcpkg/vcpkg.exe)) {
    vcpkg/bootstrap-vcpkg.bat
}
Set-Content -Path vcpkg\ports\stencil\use_source_path -Value $PSScriptRoot\..
vcpkg\vcpkg.exe install ("stencil:" + $triplet)
$vcpkgroot = $(Resolve-Path vcpkg)
function TestWithVCPKG {
    param (
        [ValidateSet("Debug", "Release")] $config
    )
    if ((Test-Path Test-Debug)) {
        Get-Item Test-Debug | Remove-Item -Recurse 
    }
    mkdir Test-Debug
    Push-Location Test-Debug
    cmake.exe -DVCPKG_ROOT:PATH=$vcpkgroot -DVCPKG_VERBOSE:BOOL=ON ("-DVCPKG_TARGET_TRIPLET:STRING=" + $triplet) $PSScriptRoot\vcpkg
    cmake.exe --build . --config $config -j
    ctest . -C $config 
    Pop-Location
    Get-Item Test-Debug | Remove-Item -Recurse
}

TestWithVCPKG "Debug"
TestWithVCPKG "Release"

