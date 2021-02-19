import argparse
import os
from os.path import pardir
import shutil
import subprocess
import sys

parser = argparse.ArgumentParser(description="Test VCPKG Workflow")
parser.add_argument("--workdir", type=str, default=".", help="Root")
parser.add_argument("--triplet", type=str, default=None, help="Triplet")
args = parser.parse_args()
workdir = os.path.abspath(args.workdir)
os.makedirs(workdir, exist_ok=True)
vcpkgroot = os.path.join(workdir, "vcpkg")

if not os.path.exists(vcpkgroot):
    subprocess.check_call(["git", "clone", "-q", "https://github.com/ankurverma85/vcpkg.git", "--branch", "ankurv/stencil", "--depth", "1"], cwd=workdir)

stencilportdir = os.path.join(vcpkgroot, "ports", "stencil")
scriptdir = os.path.abspath(os.path.dirname(__file__))
bootstrapscript = "bootstrap-vcpkg.bat" if sys.platform == "win32" else "bootstrap-vcpkg.sh"
defaulttriplet = "x64-windows" if sys.platform == "win32" else "x64-linux"
triplet = args.triplet or defaulttriplet
if os.path.exists(stencilportdir):
    shutil.rmtree(stencilportdir)
shutil.copytree(os.path.join(scriptdir, "vcpkg", "port.stencil"), stencilportdir)
with open(os.path.join(stencilportdir, "use_source_path"), "w") as f:
    f.write(os.path.abspath(os.path.dirname(scriptdir)))

subprocess.check_call(os.path.join("vcpkg", bootstrapscript), shell=True, cwd=workdir)
vcpkgexe = shutil.which("vcpkg", path=vcpkgroot)
subprocess.check_call([vcpkgexe, "install", "stencil:" + triplet])

def TestVcpkgBuild(config):
    testdir =  os.path.join(workdir, "Test-" + config)
    if os.path.exists(testdir): shutil.rmtree(testdir)
    os.makedirs(testdir)
    cmakebuildextraargs =  (["--config", config] if sys.platform == "win32" else [])
    ctestextraargs =  (["-C", config] if sys.platform == "win32" else [])
    subprocess.check_call(["cmake",
        "-DCMAKE_BUILD_TYPE=" + config, 
        "-DVCPKG_ROOT:PATH=" + vcpkgroot, 
        "-DVCPKG_TARGET_TRIPLET=" + triplet,
        "-DVCPKG_VERBOSE:BOOL=ON", 
        os.path.join(scriptdir, "vcpkg")], cwd=testdir)
    subprocess.check_call(["cmake", "--build", ".", "-j"] +cmakebuildextraargs, cwd=testdir)
    subprocess.check_call(["ctest", "."] + ctestextraargs, cwd=testdir)

TestVcpkgBuild("Debug")
TestVcpkgBuild("Release")

