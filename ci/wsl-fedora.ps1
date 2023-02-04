# https://kojipkgs.fedoraproject.org//packages/Fedora-Container-Base/37/20221110.n.0/images/Fedora-Container-Base-37-20221110.n.0.x86_64.tar.xz
# Extract layer.tar
# wsl --import f37 e:\wsl.f37 E:\layer.tar
# wsl 
# adduser ankurv; usermod -aG wheel ankurv; dnf install -y vim passwd util-linux 
# edit wsl.conf
#   [user]
#   default = ankurv
#   systemd=true
