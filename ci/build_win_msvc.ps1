$ErrorActionPreference = "Stop"
$vs2019_path = (@()+(&"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -version 16.0 -property installationpath))[-1]
Import-Module (Join-Path $vs2019_path "Common7\Tools\Microsoft.VisualStudio.DevShell.dll")
Enter-VsDevShell -VsInstallPath $vs2019_path -SkipAutomaticLocation -DevCmdArguments "-host_arch=amd64 -arch=amd64"
$scriptdir = (Split-Path $MyInvocation.MyCommand.Path -Parent) 

cmake.exe -DCMAKE_BUILD_TYPE=Debug $scriptdir/..
cmake.exe --build . -j --target package
ctest.exe -C Debug