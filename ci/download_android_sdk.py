#!/usr/bin/python3
# pylint: disable=invalid-name

import html.parser
import os
import pathlib
import re
import shutil
import ssl
import stat
import subprocess
import sys
import time
import urllib.parse
import urllib.request
from typing import Callable, Optional


class HTMLUrlExtractor(html.parser.HTMLParser):
    def __init__(self, url):
        text = urllib.request.urlopen(url, timeout=10, context=ssl._create_unverified_context()).read().decode("utf-8")
        self.baseurl = url
        self.urls = {}
        self.href = None
        self.text = None
        super(HTMLUrlExtractor, self).__init__()
        self.feed(text)

    def handle_starttag(self, tag, attrs):
        if tag == "a":
            self.text = ""
            self.href = next((urllib.parse.urljoin(self.baseurl, attr[1]) for attr in attrs if attr[0] == "href"), None)

    def handle_endtag(self, tag):
        if self.href is not None:
            # print(self.href, self.text)
            self.urls[self.href] = self.text
        self.href = None
        self.text = None

    def handle_data(self, data):
        if self.href is not None:
            self.text = data

    def error(self, _message: str) -> None:
        pass


def _search_filename(path: pathlib.Path, name) -> Optional[pathlib.Path]:
    for fpath, _dirs, fls in os.walk(path):
        if name in fls:
            return pathlib.Path(fpath) / name
    return None


def _search_exe(bindir: pathlib.Path, binname) -> Optional[pathlib.Path]:
    if sys.platform == "win32":
        path = _search_filename(bindir, binname + ".bat") or _search_filename(bindir,
                                                                              binname + ".cmd") or _search_filename(bindir, binname + ".exe")
    elif sys.platform == "linux":
        path = _search_filename(bindir, binname + ".sh") or _search_filename(bindir, binname)
        if path:
            os.chmod(path, os.stat(path).st_mode | stat.S_IEXEC)
    return path


def _move_up(path: pathlib.Path):
    for p in os.scandir(path):
        shutil.move(p.path, path.parent)
    os.rmdir(path)


def _download_or_get_Binary(binname, bindir: pathlib.Path, downloadFn: Callable[[pathlib.Path], None]) -> pathlib.Path:
    exe = None  # shutil.which(binname)
    if exe is not None:
        if sys.platform == "win32" and exe.parent.name != "system32":
            return exe
    os.makedirs(bindir, exist_ok=True)
    exe = _search_exe(bindir, binname)
    if exe is not None:
        AddToPath(exe.parent)
        return exe
    downloadFn(bindir)
    exe = _search_exe(bindir, binname)
    if exe is not None:
        AddToPath(exe.parent)
        return exe
    raise Exception(f"Cannot find or download {binname} in {bindir}")


def AddToPath(path: pathlib.Path):
    path = path.absolute()
    if path not in os.environ['PATH'].split(os.pathsep):
        os.environ['PATH'] = os.pathsep.join([str(path), os.environ['PATH']])


def DownloadSdkManager(path: pathlib.Path):
    sdkpath = os.path.join(path, 'sdk')
    os.makedirs(sdkpath, exist_ok=True)
    urls = HTMLUrlExtractor("https://developer.android.com/studio").urls
    ossuffix = {"linux": "linux", "win32": "win"}[sys.platform]
    pattern = f"https://dl.google.com/android/repository/commandlinetools-{ossuffix}.*.zip"
    url = [u for u in urls if re.match(pattern, u)][0]
    downloadtofile = os.path.join(path, "downloads", "commandlinetools.zip")
    if not os.path.exists(downloadtofile):
        print(f"Download {url} to {downloadtofile}")
        urllib.request.urlretrieve(url, downloadtofile)
    shutil.unpack_archive(downloadtofile, sdkpath)
    AcceptSDKLicenses(path)


def DownloadAndroidStudio(path: pathlib.Path):
    ossuffix = {"linux": "linux", "win32": "win"}[sys.platform]
    ext = {"linux": "tar.gz", "win32": "zip"}[sys.platform]
    urls = HTMLUrlExtractor("https://developer.android.com/studio").urls
    ossuffix = {"linux": "linux", "win32": "windows"}[sys.platform]
    pattern = f"https://redirector.gvt1.com/edgedl/android/studio/.*/android-studio-.*-{ossuffix}.{ext}"
    url = [u for u in urls if re.match(pattern, u)][0]
    downloadtofile = os.path.join(path, "downloads", f"studio.{ext}")
    os.makedirs(os.path.dirname(downloadtofile), exist_ok=True)
    if not os.path.exists(downloadtofile):
        print(f"Download {url} to {downloadtofile}")
        urllib.request.urlretrieve(url, downloadtofile)
    shutil.unpack_archive(downloadtofile, path)


def AcceptSDKLicenses(path: pathlib.Path):
    sdkpath = os.path.join(path, 'sdk')
    cmd = [str(_download_or_get_Binary('sdkmanager', path, DownloadSdkManager)), f"--sdk_root={sdkpath}", '--licenses']
    sys.stderr.write(" ".join(cmd) + "\n")
    proc = subprocess.Popen(cmd, stdin=subprocess.PIPE)
    while proc.poll() is None:
        time.sleep(1)
        proc.communicate(input=b'y\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\n')


def GetAndroidSdkManager(path: pathlib.Path) -> pathlib.Path:
    return _download_or_get_Binary('sdkmanager', path, DownloadSdkManager)


def GetAndroidStudio(path: pathlib.Path) -> pathlib.Path:
    return _download_or_get_Binary('studio', path, DownloadAndroidStudio)


def DownloadJava(path: pathlib.Path):
    return DownloadAndroidStudio(path)  # Just use java from android studio


def GetJava(path: pathlib.Path) -> pathlib.Path:
    return _download_or_get_Binary('java', path, DownloadJava)


def GetJarSigner(path):
    return _download_or_get_Binary('jarsigner', path, DownloadJava)


def DownloadTo(path: pathlib.Path):
    java = GetJava(path)
    sdkmanager = GetAndroidSdkManager(path)
    sdkpath = path / 'sdk'
    os.environ["PATH"] = os.pathsep.join([str(java.parent), str(sdkmanager.parent), os.environ["PATH"]])
    dirs = {''}
    for d in os.scandir(sdkpath):
        dirs.add(d.name)
    packages = ["ndk-bundle", "ndk;23.0.7599858", "build-tools;31.0.0", "platform-tools", "platforms;android-31"]

    def IsNeeded():
        for p in packages:
            if not p.split(';', maxsplit=1)[0] in dirs:
                return True
        return False
    retval = {
        "ndk": (sdkpath / "ndk-bundle").absolute(),
        "java_home": java.parent.parent,
        "sdk_root": sdkpath
    }
    if not IsNeeded():
        return retval
    cmd = [str(sdkmanager), f"--sdk_root={sdkpath}"] + packages
    sys.stderr.write(" ".join(cmd) + "\n")
    subprocess.check_call(cmd)
    AcceptSDKLicenses(path)
    return retval


if __name__ == "__main__":
    DownloadTo(pathlib.Path(sys.argv[1]))
