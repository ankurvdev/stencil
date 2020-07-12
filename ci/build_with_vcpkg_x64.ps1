$vs2019_path = (@()+(&"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -version 16.0 -property installationpath))[-1]
Import-Module (Join-Path $vs2019_path "Common7\Tools\Microsoft.VisualStudio.DevShell.dll")
Enter-VsDevShell -VsInstallPath $vs2019_path -SkipAutomaticLocation -DevCmdArguments "-host_arch=amd64 -arch=amd64"
$ErrorActionPreference = "Stop"
try {
    git clone -q https://github.com/Microsoft/vcpkg.git
    git -C vcpkg apply  --ignore-space-change --ignore-whitespace --whitespace=fix $PSScriptRoot\vcpkg\vcpkg.stencil.patch
}
catch{
    echo "Git commands somehow are causing issues. Fix me later (TODO)"
}

Set-Item ENV:\VCPKG_ROOT $(Resolve-Path vcpkg)
vcpkg/bootstrap-vcpkg.bat
Set-Content -Path vcpkg\ports\stencil\use_source_path -Value $PSScriptRoot\..
vcpkg\vcpkg install stencil:x64-windows

Get-Content vcpkg\buildtrees\stencil\install-x64-windows-rel-out.log
Get-Content vcpkg\buildtrees\stencil\install-x64-windows-dbg-out.log

cmake.exe -DVCPKG_ROOT:FILEPATH=$(Resolve-Path vcpkg) -DVCPKG_TARGET_TRIPLET=x64-windows $PSScriptRoot\vcpkg
cmake.exe --build . -j
