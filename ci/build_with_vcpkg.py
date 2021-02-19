import argparse
import os
from os.path import pardir
import shutil
import subprocess
import sys

parser = argparse.ArgumentParser(description="Test VCPKG WOrkflow")
parser.add_argument("--workdir", type=str, default=".", help="Root")
parser.add_argument("--triplet", type=str, default=None, help="Triplet")
args = parser.parse_args()
if not os.path.exists("vcpkg"):
    subprocess.check_call("git clone -q https://github.com/ankurverma85/vcpkg.git --branch ankurv/stencil --depth 1", cwd=args.workdir)

vcpkgroot = os.path.join(args.workdir, "vcpkg")
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

subprocess.check_call(os.path.join("vcpkg", bootstrapscript), shell=True)
vcpkgexe = shutil.which("vcpkg", path=vcpkgroot)
subprocess.check_call([vcpkgexe, "install", "stencil"])

def TestVcpkgBuild(config):
    testdir =  os.path.join(args.workdir, "Test-" + config)
    if os.path.exists(testdir): shutil.rmtree(testdir)
    os.makedirs(testdir)
    subprocess.check_call(["cmake",
        "-DCMAKE_BUILD_TYPE=" + config, 
        "-DVCPKG_ROOT:PATH=" + vcpkgroot, 
        "-DVCPKG_TARGET_TRIPLET=" + triplet,
        "-DVCPKG_VERBOSE:BOOL=ON", 
        os.path.join(scriptdir, "vcpkg")], cwd=testdir)
    subprocess.check_call("cmake --build . -j --target package" + (("--config " + config) if sys.platform == "win32" else ""), cwd=testdir)
    subprocess.check_call("ctest . " + (("-C " + config) if sys.platform == "win32" else ""), cwd=testdir)

TestVcpkgBuild("Debug")
TestVcpkgBuild("Release")

