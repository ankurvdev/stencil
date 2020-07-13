# Install build pre-requisites.
sudo apt-get update && \
sudo apt-get install --yes --no-install-recommends --quiet \
    build-essential \
    gnupg \
    lsb-release \
    software-properties-common

UBUNTU_CODENAME=$(lsb_release --codename --short)
UBUNTU_VERSION=$(lsb_release --release --short)

# Configure package repository for clang toolchain.
wget -q -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
apt-key adv --list-public-keys --with-fingerprint --with-colons | grep -i 6084F3CF814B57C1CF12EFD515CF4D18AF4F7421
sudo apt-add-repository "deb http://apt.llvm.org/${UBUNTU_CODENAME}/ llvm-toolchain-${UBUNTU_CODENAME}-10 main"

# CMake repositoryn for latest cmake
wget -q -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | sudo apt-key add -
sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ ${UBUNTU_CODENAME} main"

# Required for providing latest stl headers (libstdc++-)
# Configure package repository for gcc toolchain.
# https://wiki.ubuntu.com/ToolChain
sudo add-apt-repository "ppa:ubuntu-toolchain-r/test"

# Install build pre-requisites.
sudo apt-get update && \
sudo apt-get install --yes --no-install-recommends --quiet \
    clang-10 \
    clang-format-10 \
    libstdc++-10-dev \
    cmake flex bison gettext \


sudo update-alternatives --install /usr/bin/cc cc /usr/bin/clang-10 100
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-10 100
