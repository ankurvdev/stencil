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
        path = SearchFilename(bindir, binname + ".exe") or SearchFilename(bindir, binname + ".bat") or SearchFilename(bindir, binname + ".cmd")
    elif sys.platform == "linux":
        path = SearchFilename(bindir, binname)
        if path:
            os.chmod(path, os.stat(path).st_mode | stat.S_IEXEC)
    return path


def MoveUp(dir):
    for p in os.scandir(dir):
        shutil.move(p.path, os.path.join(dir, ".."))
    os.rmdir(dir)


def GetBinary(binname, bindir):
    exe = shutil.which(binname)
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
    proc = subprocess.Popen([GetBinary('sdkmanager', path), f"--sdk_root={path}", '--licenses'], stdin=subprocess.PIPE)
    while proc.poll() is None:
        time.sleep(1)
        proc.communicate(input=b'y\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\ny\n')

def AddToPath(dir):
    dir = os.path.abspath(dir)
    if dir not in os.environ['PATH'].split(os.pathsep):
        os.environ['PATH'] = os.pathsep.join([dir, os.environ['PATH']])

def DownloadSdkManager(path):
    urls = HTMLUrlExtractor("https://developer.android.com/studio").urls
    ossuffix = {"linux" : "linux", "win32" : "win"}[sys.platform]
    pattern =  f"https://dl.google.com/android/repository/commandlinetools-{ossuffix}.*.zip"
    url = [u for u in urls.keys() if re.match(pattern, u)][0]

    downloadtofile = os.path.join(path, "downloads", "commandlinetools.zip")
    os.makedirs(os.path.dirname(downloadtofile), exist_ok=True)
    if not os.path.exists(downloadtofile):
        print(f"Download {url} to {downloadtofile}")
        urllib.request.urlretrieve(url, downloadtofile)
    shutil.unpack_archive(downloadtofile, path)
    AcceptSDKLicenses(path)

def GetAndroidSdkManager(path):
    return GetBinary('sdkmanager', path) or DownloadSdkManager(path) or GetBinary('sdkmanager', path)

def DownloadJava(path):
    urls = HTMLUrlExtractor("https://docs.microsoft.com/en-us/java/openjdk/download").urls
    ossuffix = {"linux" : "linux", "win32" : "windows"}[sys.platform]
    pattern =  f"https://aka.ms/download-jdk/microsoft-jdk-16.*{ossuffix}-x64.*"
    url = [u for u in urls.keys() if re.match(pattern, u)][0]
    ext = {"linux" : "tar.gz", "win32" : "zip"}[sys.platform]
    downloadtofile = os.path.join(path, "downloads", f"jdk.{ext}")
    os.makedirs(os.path.dirname(downloadtofile), exist_ok=True)
    if not os.path.exists(downloadtofile):
        print(f"Download {url} to {downloadtofile}")
        urllib.request.urlretrieve(url, downloadtofile)
    shutil.unpack_archive(downloadtofile, path)

def GetJava(path):
    return GetBinary('java', path) or DownloadJava(path) or GetBinary('java', path)

def DownloadTo(path:str):
    java = GetJava(path)
    sdkmanager = GetAndroidSdkManager(path)
    os.environ["PATH"] = os.pathsep.join([os.path.dirname(java) , os.path.dirname(sdkmanager), os.environ["PATH"]])
    AcceptSDKLicenses(path)
    subprocess.check_call([sdkmanager, f"--sdk_root={path}", "ndk-bundle", "ndk;22.1.7171670", "build-tools;31.0.0",  "platform-tools", "platforms;android-31"])
    return {"ndk": os.path.abspath(os.path.join(path, "ndk-bundle"))}

if __name__ == "__main__":
    DownloadTo(sys.argv[1])