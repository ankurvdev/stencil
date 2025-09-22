#!/usr/bin/env python3
# cppforge-sync
import argparse
import logging
import os
import pathlib
import shutil
import subprocess
import sys
import urllib.request
from pathlib import Path
import json

sys.path.append(pathlib.Path(__file__).parent.parent.as_posix())

import git

parser = argparse.ArgumentParser(description="Test VCPKG Workflow")
parser.add_argument("--verbose", action="store_true", help="Clean")
parser.add_argument("--vcpkg", type=Path, default=None, help="Repository")
parser.add_argument("--port", type=Path, default=None, help="Repository")
parser.add_argument("--branch", type=str, default=None, help="Repository")
args = parser.parse_args()
portdir = Path(args.vcpkg) / "ports" / args.port
if not portdir.exists():
    raise FileNotFoundError(f"Port directory {portdir} does not exist")
gitter = git.Git()
gitter.set_root(args.vcpkg)
curbranch = gitter.current_branch()
if gitter.dirty():
    raise ValueError(f"Branch {curbranch} dirty")
if args.branch and curbranch != args.branch:
    gitter.checkout(args.branch)
prevvcpkgjson = json.loads((portdir / "vcpkg.json").read_text())
portgithub = prevvcpkgjson["homepage"]
prevver = prevvcpkgjson["version"]
prevportversion = int(prevvcpkgjson.get("port-version", 0))

if not portgithub.startswith("https://github.com/ankurvdev"):
    raise ValueError(f"Port {args.port} {portgithub} not supported ")
portgithubname = portgithub.split("/")[-1]
for portf in portdir.glob("*"):
    if portf.is_file():
        rawurl = (
            f"https://raw.githubusercontent.com/ankurvdev/{portgithubname}/vcpkg/ci/vcpkg-additional-ports/{portgithubname}/{portf.name}"
        )
        urllib.request.urlretrieve(rawurl, portf)
newver = json.loads((portdir / "vcpkg.json").read_text())["version"]
if newver == prevver:
    oldcontent = list((portdir / "vcpkg.json").read_text().splitlines())
    oldcontent.insert(3, f'  "port-version": {prevportversion + 1},')
    newcontent = "\n".join(oldcontent)
    (portdir / "vcpkg.json").write_text(newcontent, newline="\n")
gitter.cmd(["add", portdir.as_posix()])
gitter.cmd(["commit", "-m", f"Update {args.port}"])
gitter.cmd(["push"])
if args.branch:
    gitter.checkout(curbranch)
    gitter.cmd(["merge", args.branch])
    gitter.cmd(["push"])
    gitter.cmd(["branch", "-d", args.branch])
