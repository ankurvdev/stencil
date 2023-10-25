# https://kojipkgs.fedoraproject.org//packages/Fedora-Container-Base/37/20221110.n.0/images/Fedora-Container-Base-37-20221110.n.0.x86_64.tar.xz
# Extract layer.tar
# wsl --import f37 e:\wsl.f37 E:\layer.tar
# wsl 
# adduser ankurv; usermod -aG wheel ankurv; dnf install -y vim passwd util-linux 
# edit wsl.conf
#   [user]
#   default = ankurv
#   systemd=true
wsl --import f37 e:\wsl.f37 E:\layer.tar
wsl
dnf install -y vim passwd util-linux
adduser ankurv; usermod -aG wheel ankurv; passwd ankurv
printf "[user]\ndefault = ankurv\nsystemd=true\n" > /etc/wsl.conf
dnf install -y openssh-clients cmake gcc-c++ gdb git python-unversioned-command python3-pip procps-ng
git config --global user.name "Ankur"
git config --global user.email "ankurv@verma"

# VCPKG
dnf install -y zip tar xz bzip2 # core
dnf install -y flex bison # stencil
dnf install -y perl-FindBin perl-IPC-Cmd perl-File-Compare perl-File-Copy # openssl
dnf install -y bison # gettext
dnf install -y autoconf automake libtool # freexl	
dnf install -y autoconf-archive # icu
dnf install -y systemd-devel # libusb
dnf install -y mesa-libGL-devel libxcb-devel libXrender-devel xcb-util-*-devel xcb-util-devel libxkbcommon-devel libxkbcommon-x11-devel perl-English # qtbase