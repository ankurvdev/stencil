$ErrorActionPreference = "Stop"
$vs2019_path = (@()+(&"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -version 16.0 -property installationpath))[-1]
Import-Module (Join-Path $vs2019_path "Common7\Tools\Microsoft.VisualStudio.DevShell.dll")
Enter-VsDevShell -VsInstallPath $vs2019_path -SkipAutomaticLocation -DevCmdArguments "-host_arch=amd64 -arch=amd64"
$scriptdir = (Split-Path $MyInvocation.MyCommand.Path -Parent) 


git clone https://github.com/Microsoft/vcpkg.git
git -C vcpkg apply  --ignore-space-change --ignore-whitespace $scriptdir\vcpkg\vcpkg.stencil.patch
vcpkg/bootstrap-vcpkg.bat

Set-Content -Path vcpkg\ports\stencil\use_source_path -Value $scriptdir\..
vcpkg\vcpkg install stencil
cmake.exe -G Ninja -DVCPKG_ROOT:FILEPATH=$(Resolve-Path vcpkg) $scriptdir\vcpkg
cmake.exe --build . -j
ctest.exe .