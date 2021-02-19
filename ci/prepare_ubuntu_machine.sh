
if ((EUID != 0)); then
  echo "Rerunning as root - current Effective UID = ${EUID}"
  id
  sudo bash $0 $*
  exit 0
fi

# Exit immediately if a command exits with a non-zero status.
set -e
set -o pipefail

# Install build pre-requisites.
apt-get update && \
apt-get install --yes --no-install-recommends --quiet \
    build-essential \
    gnupg \
    lsb-release \
    software-properties-common

CLANG_VERSION=12
STDLIB_VERSION=11

UBUNTU_CODENAME=$(lsb_release --codename --short)
UBUNTU_VERSION=$(lsb_release --release --short)

# Configure package repository for clang toolchain.
wget -q -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
apt-key adv --list-public-keys --with-fingerprint --with-colons | grep -i 6084F3CF814B57C1CF12EFD515CF4D18AF4F7421
apt-add-repository "deb http://apt.llvm.org/${UBUNTU_CODENAME}/ llvm-toolchain-${UBUNTU_CODENAME}-10 main"

# CMake repositoryn for latest cmake
wget -q -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add -
apt-add-repository "deb https://apt.kitware.com/ubuntu/ ${UBUNTU_CODENAME} main"

# Required for providing latest stl headers (libstdc++-)
# Configure package repository for gcc toolchain.
# https://wiki.ubuntu.com/ToolChain
add-apt-repository "ppa:ubuntu-toolchain-r/test"

# Install build pre-requisites.
apt-get update && \
apt-get install --yes --no-install-recommends --quiet \
    clang-${CLANG_VERSION} \
    clang-format-${CLANG_VERSION} \
    libstdc++-${STDLIB_VERSION}-dev \
    cmake flex bison gettext \

update-alternatives --install /usr/bin/cc cc /usr/bin/clang-${CLANG_VERSION} 100
update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-${CLANG_VERSION} 100

# Print out tool version info
for tool in cmake cc gcc g++ clang-${CLANG_VERSION}; do
  echo .
  echo "-- ${tool} version"
  command -v ${tool}
  ${tool} --version
done