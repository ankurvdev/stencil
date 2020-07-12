$vs2019_path = (@()+(&"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -version 16.0 -property installationpath))[-1]
Import-Module (Join-Path $vs2019_path "Common7\Tools\Microsoft.VisualStudio.DevShell.dll")
Enter-VsDevShell -VsInstallPath $vs2019_path -SkipAutomaticLocation -DevCmdArguments "-host_arch=x86 -arch=x86"
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
vcpkg\vcpkg install stencil:x86-windows
#-DVCPKG_TARGET_TRIPLET=x86-windows is the default
cmake.exe -DVCPKG_ROOT:FILEPATH=$(Resolve-Path vcpkg) $PSScriptRoot\vcpkg
cmake.exe --build . -j
