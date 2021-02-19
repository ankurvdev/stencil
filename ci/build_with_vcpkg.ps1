param (
    [ValidateSet("x86", "x64")] $arch
)
. $PSScriptRoot\start_devenv.ps1
Start-Devenv $arch
$ErrorActionPreference = "Stop"
try {
    git clone -q https://github.com/Microsoft/vcpkg.git
    git -C vcpkg apply  --ignore-space-change --ignore-whitespace --whitespace=fix $PSScriptRoot\vcpkg\vcpkg.stencil.patch
}
catch{
    Write-Content "Git commands somehow are causing issues. Fix me later (TODO)"
}

Set-Item ENV:\VCPKG_ROOT $(Resolve-Path vcpkg)
vcpkg/bootstrap-vcpkg.bat
Set-Content -Path vcpkg\ports\stencil\use_source_path -Value $PSScriptRoot\..
vcpkg\vcpkg install stencil:$arch-windows

Get-Content vcpkg\buildtrees\stencil\install-$arch-windows-rel-out.log
Get-Content vcpkg\buildtrees\stencil\install-$arch-windows-dbg-out.log

cmake.exe -DVCPKG_ROOT:FILEPATH=$(Resolve-Path vcpkg) -DVCPKG_TARGET_TRIPLET=$arch-windows $PSScriptRoot\vcpkg
cmake.exe --build . -j
