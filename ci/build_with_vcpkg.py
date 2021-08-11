import argparse
import os
from os.path import pardir
import shutil
import subprocess
import sys

parser = argparse.ArgumentParser(description="Test VCPKG Workflow")
parser.add_argument("--workdir", type=str, default=".", help="Root")
parser.add_argument("--tool-triplet", type=str, default=None, help="Triplet")
parser.add_argument("--runtime-triplet", type=str, default=None, help="Triplet")
args = parser.parse_args()
workdir = os.path.abspath(args.workdir)
os.makedirs(workdir, exist_ok=True)
vcpkgroot = os.path.join(workdir, "vcpkg")
androidroot = os.path.join(workdir, "android")

if not os.path.exists(vcpkgroot):
    subprocess.check_call(["git", "clone", "-q", "https://github.com/ankurverma85/vcpkg.git", "--branch", "ankurv/stencil", "--depth", "1"], cwd=workdir)

scriptdir = os.path.abspath(os.path.dirname(__file__))
bootstrapscript = "bootstrap-vcpkg.bat" if sys.platform == "win32" else "bootstrap-vcpkg.sh"
defaulttriplet = "x64-windows-static" if sys.platform == "win32" else "x64-linux"
tool_triplet = args.tool_triplet or defaulttriplet
runtime_triplet = args.runtime_triplet or defaulttriplet

myenv = os.environ.copy()
myenv['VCPKG_OVERLAY_PORTS'] = os.path.join(scriptdir, 'vcpkg-additional-ports')
myenv['VCPKG_KEEP_ENV_VARS'] = 'VCPKG_STENCIL_SOURCE_PATH;ANDROID_NDK_HOME'

if "android" in tool_triplet or "android" in runtime_triplet:
    import download_android_sdk
    paths = download_android_sdk.DownloadTo(os.path.join(workdir, "android"))
    myenv['ANDROID_NDK_HOME'] = paths['ndk']

subprocess.check_call(os.path.join("vcpkg", bootstrapscript), shell=True, cwd=workdir, env=myenv)
vcpkgexe = shutil.which("vcpkg", path=vcpkgroot)
subprocess.check_call([vcpkgexe, "install", "stencil:" + tool_triplet], env=myenv)
subprocess.check_call([vcpkgexe, "install", "stencil:" + runtime_triplet], env=myenv)

def TestVcpkgBuild(config):
    testdir =  os.path.join(workdir, "Test-" + config)
    if os.path.exists(testdir): shutil.rmtree(testdir)
    os.makedirs(testdir)
    cmakebuildextraargs =  (["--config", config] if sys.platform == "win32" else [])
    ctestextraargs =  (["-C", config] if sys.platform == "win32" else [])
    subprocess.check_call(["cmake",
        "-DCMAKE_BUILD_TYPE=" + config, 
        "-DVCPKG_ROOT:PATH=" + vcpkgroot, 
        "-DVCPKG_TARGET_TRIPLET=" + runtime_triplet,
        "-DVCPKG_VERBOSE:BOOL=ON", 
        os.path.join(scriptdir, "vcpkg")], cwd=testdir)
    subprocess.check_call(["cmake", "--build", ".", "-j"] +cmakebuildextraargs, cwd=testdir)
    subprocess.check_call(["ctest", "."] + ctestextraargs, cwd=testdir)

TestVcpkgBuild("Debug")
TestVcpkgBuild("Release")

