if not defined DevEnvDir (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
)
if not defined BuildConfig (set BuildConfig="Debug")
cmake.exe ^
	-G "Ninja" ^
	-DCMAKE_CXX_COMPILER="cl.exe" ^
	-DCMAKE_C_COMPILER="cl.exe" ^
	-DCMAKE_BUILD_TYPE=%BuildConfig% ^
	-DCMAKE_MAKE_PROGRAM="C:/PROGRAM FILES (X86)/MICROSOFT VISUAL STUDIO/2019/ENTERPRISE/COMMON7/IDE/COMMONEXTENSIONS/MICROSOFT/CMAKE/Ninja/ninja.exe" ^
	-DCMAKE_MT:FILEPATH="C:/Program Files (x86)/Windows Kits/10/bin/10.0.18362.0/x64/mt.exe" ^
	-DCMAKE_RC_COMPILER:FILEPATH="C:/Program Files (x86)/Windows Kits/10/bin/10.0.18362.0/x64/rc.exe"^
	%~dp0\..
cmake.exe --build . -j --target package
ctest.exe