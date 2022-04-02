import argparse
import os
import pathlib
import shutil
import subprocess
import sys

if sys.platform == "win32" and not "VSINSTALLDIR" in os.environ.keys():
    subprocess.check_call(['powershell', os.path.join(os.path.dirname(__file__), "run_in_devenv.ps1"),
                          "x64", sys.executable, __file__] + sys.argv[1:])
    sys.exit(0)

parser = argparse.ArgumentParser(description="Test VCPKG Workflow")
parser.add_argument("--workdir", type=str, default=".", help="Root")
parser.add_argument("--host-triplet", type=str, default=None, help="Triplet")
parser.add_argument("--runtime-triplet", type=str, default=None, help="Triplet")
args = parser.parse_args()
workdir = pathlib.Path(args.workdir).absolute()
os.makedirs(workdir, exist_ok=True)
vcpkgroot = (workdir / "vcpkg")
androidroot = (workdir / "android")

if not vcpkgroot.exists():
    subprocess.check_call(["git", "clone", "-q", "https://github.com/ankurverma85/vcpkg.git",
                          "--branch", "ankurv/stencil", "--depth", "1"], cwd=workdir.as_posix())

scriptdir = pathlib.Path(__file__).parent.absolute()
bootstrapscript = "bootstrap-vcpkg.bat" if sys.platform == "win32" else "bootstrap-vcpkg.sh"
defaulttriplet = "x64-windows-static" if sys.platform == "win32" else "x64-linux"
host_triplet = args.host_triplet or defaulttriplet
runtime_triplet = args.runtime_triplet or defaulttriplet

myenv = os.environ.copy()
myenv['VCPKG_OVERLAY_PORTS'] = (scriptdir / 'vcpkg-additional-ports').as_posix()
myenv['VCPKG_KEEP_ENV_VARS'] = 'VCPKG_USE_STENCIL_SRC_DIR;ANDROID_NDK_HOME'
myenv['VCPKG_USE_STENCIL_SRC_DIR'] = scriptdir.parent.as_posix()
if "android" in host_triplet or "android" in runtime_triplet:
    import download_android_sdk
    paths = download_android_sdk.DownloadTo((workdir / "android"))
    myenv['ANDROID_NDK_HOME'] = paths['ndk'].as_posix()

subprocess.check_call((pathlib.Path("vcpkg") / bootstrapscript).as_posix(), shell=True, cwd=workdir, env=myenv)
vcpkgexe = pathlib.Path(shutil.which("vcpkg", path=vcpkgroot) or "")
subprocess.check_call([vcpkgexe, "install", "stencil:" + host_triplet], env=myenv)
subprocess.check_call([vcpkgexe, "install", "stencil:" + runtime_triplet], env=myenv)


def test_vcpkg_build(config: str, host_triplet: str, runtime_triplet: str):
    testdir = workdir / f"{runtime_triplet}_Test_{config}"
    if testdir.exists():
        shutil.rmtree(testdir.as_posix())
    testdir.mkdir()
    cmakebuildextraargs = (["--config", config] if sys.platform == "win32" else [])
    cmakeconfigargs: list[str] = []
    if "android" in runtime_triplet:
        cmakeconfigargs += [
            "-DCMAKE_TOOLCHAIN_FILE:PATH=" + myenv['ANDROID_NDK_HOME'] + "/build/cmake/android.toolchain.cmake",
            "-DANDROID=1",
            "-DANDROID_NATIVE_API_LEVEL=21",
        ]
        if sys.platform == "win32":
            cmakeconfigargs += ["-G", "Ninja"]

    ctestextraargs = (["-C", config] if sys.platform == "win32" else [])
    cmd: list[str] = ["cmake",
                      "-DCMAKE_BUILD_TYPE=" + config,
                      "-DVCPKG_ROOT:PATH=" + vcpkgroot.as_posix(),
                      "-DVCPKG_HOST_TRIPLET=" + host_triplet,
                      "-DVCPKG_TARGET_TRIPLET=" + runtime_triplet,
                      "-DVCPKG_VERBOSE:BOOL=ON"] + cmakeconfigargs + [
        (scriptdir / "sample").as_posix()]
    subprocess.check_call(cmd, cwd=testdir.as_posix())
    subprocess.check_call(["cmake", "--build", ".", "-j"] + cmakebuildextraargs, cwd=testdir)
    if runtime_triplet == host_triplet:
        subprocess.check_call(["ctest", "."] + ctestextraargs, cwd=testdir)


test_vcpkg_build("Debug", host_triplet, runtime_triplet, )
test_vcpkg_build("Release", host_triplet, runtime_triplet)
