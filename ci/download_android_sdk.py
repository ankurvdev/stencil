#!/usr/bin/python3
import os
from posixpath import dirname
import sys
import stat
import subprocess
import sys
import urllib.request
import urllib.parse
import ssl
import pathlib
import zipfile
import shutil
import platform
import re
import html.parser
import time
import threading


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


def SearchFilename(dir, name):
    for path, dirs, fls in os.walk(dir):
        if name in fls:
            return os.path.join(path, name)
    return None


def SearchExecutable(bindir, binname):
    if sys.platform == "win32":
        path = SearchFilename(bindir, binname + ".bat") or SearchFilename(bindir, binname + ".cmd") or SearchFilename(bindir, binname + ".exe")
    elif sys.platform == "linux":
        path = SearchFilename(bindir, binname + ".sh") or SearchFilename(bindir, binname)
        if path:
            os.chmod(path, os.stat(path).st_mode | stat.S_IEXEC)
    return path


def MoveUp(dir):
    for p in os.scandir(dir):
        shutil.move(p.path, os.path.join(dir, ".."))
    os.rmdir(dir)


def GetBinary(binname, bindir):
    exe = None # shutil.which(binname)
    if exe is not None:
        if sys.platform == "win32" and os.path.basename(os.path.dirname(exe)) != "system32":
            return exe
    os.makedirs(bindir, exist_ok=True)
    exe = SearchExecutable(bindir, binname)
    if exe is not None:
        AddToPath(os.path.dirname(exe))
        return exe
    return None


def AcceptSDKLicenses(path):
    sdkpath = os.path.join(path, 'sdk')
    cmd = [GetBinary('sdkmanager', path), f"--sdk_root={sdkpath}", '--licenses']
    sys.stderr.write(" ".join(cmd) + "\n")
    proc = subprocess.Popen(cmd, stdin=subprocess.PIPE)
    while proc.poll() is None:
        time.sleep(1)
        proc.communicate(input=b'y\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\n')


def AddToPath(dir):
    dir = os.path.abspath(dir)
    if dir not in os.environ['PATH'].split(os.pathsep):
        os.environ['PATH'] = os.pathsep.join([dir, os.environ['PATH']])


def DownloadSdkManager(path):
    sdkpath = os.path.join(path, 'sdk')
    os.makedirs(sdkpath, exist_ok=True)
    urls = HTMLUrlExtractor("https://developer.android.com/studio").urls
    ossuffix = {"linux": "linux", "win32": "win"}[sys.platform]
    pattern = f"https://dl.google.com/android/repository/commandlinetools-{ossuffix}.*.zip"
    url = [u for u in urls.keys() if re.match(pattern, u)][0]
    downloadtofile = os.path.join(path, "downloads", "commandlinetools.zip")
    if not os.path.exists(downloadtofile):
        print(f"Download {url} to {downloadtofile}")
        urllib.request.urlretrieve(url, downloadtofile)
    shutil.unpack_archive(downloadtofile, sdkpath)
    AcceptSDKLicenses(path)

def DownloadAndroidStudio(path):
    studiopath = path
    ossuffix = {"linux": "linux", "win32": "win"}[sys.platform]
    ext = {"linux": "tar.gz", "win32": "zip"}[sys.platform]
    urls = HTMLUrlExtractor("https://developer.android.com/studio").urls
    ossuffix = {"linux": "linux", "win32": "windows"}[sys.platform]
    pattern = f"https://redirector.gvt1.com/edgedl/android/studio/.*/android-studio-.*-{ossuffix}.{ext}"
    url = [u for u in urls.keys() if re.match(pattern, u)][0]
    downloadtofile = os.path.join(path, "downloads", f"studio.{ext}")
    os.makedirs(os.path.dirname(downloadtofile), exist_ok=True)
    if not os.path.exists(downloadtofile):
        print(f"Download {url} to {downloadtofile}")
        urllib.request.urlretrieve(url, downloadtofile)
    shutil.unpack_archive(downloadtofile, path)

def GetAndroidSdkManager(path):
    return GetBinary('sdkmanager', path) or DownloadSdkManager(path) or GetBinary('sdkmanager', path)

def GetAndroidStudio(path):
    return GetBinary('studio', path) or DownloadAndroidStudio(path) or GetBinary('studio', path)

def DownloadJava(path):
    return DownloadAndroidStudio(path) # Just use java from android studio
    urls = HTMLUrlExtractor("https://docs.microsoft.com/en-us/java/openjdk/download").urls
    ossuffix = {"linux": "linux", "win32": "windows"}[sys.platform]
    pattern = f"https://aka.ms/download-jdk/microsoft-jdk-16.*{ossuffix}-x64.*"
    url = [u for u in urls.keys() if re.match(pattern, u)][0]
    #if sys.platform == "linux":
    #    url = 'https://javadl.oracle.com/webapps/download/AutoDL?BundleId=244058_89d678f2be164786b292527658ca1605'
    ext = {"linux": "tar.gz", "win32": "zip"}[sys.platform]
    downloadtofile = os.path.join(path, "downloads", f"jdk.{ext}")
    os.makedirs(os.path.dirname(downloadtofile), exist_ok=True)
    if not os.path.exists(downloadtofile):
        print(f"Download {url} to {downloadtofile}")
        urllib.request.urlretrieve(url, downloadtofile)
    shutil.unpack_archive(downloadtofile, path)


def GetJava(path):
    return GetBinary('java', path) or DownloadJava(path) or GetBinary('java', path)

def GetJarSigner(path):
    return GetBinary('jarsigner', path) or DownloadJava(path) or GetBinary('jarsigner', path)

def DownloadTo(path: str):
    java = GetJava(path)
    sdkmanager = GetAndroidSdkManager(path)
    sdkpath = os.path.join(path, 'sdk')
    os.environ["PATH"] = os.pathsep.join([os.path.dirname(java), os.path.dirname(sdkmanager), os.environ["PATH"]])
    dirs = {''}
    for d in os.scandir(sdkpath):
        dirs.add(d.name)
    packages = ["ndk-bundle", "ndk;23.0.7599858", "build-tools;31.0.0", "platform-tools", "platforms;android-31"]
    def IsNeeded():
        for p in packages:
            if not p.split(';')[0] in dirs: return True
        return False
    retval = {
        "ndk": os.path.abspath(os.path.join(sdkpath, "ndk-bundle")),
        "java_home": os.path.dirname(os.path.dirname(java)),
        "sdk_root": sdkpath
    }
    if not IsNeeded(): return retval
    cmd = [sdkmanager, f"--sdk_root={sdkpath}"] + packages
    sys.stderr.write(" ".join(cmd) + "\n")
    subprocess.check_call(cmd)
    AcceptSDKLicenses(path)
    return retval


if __name__ == "__main__":
    DownloadTo(sys.argv[1])
