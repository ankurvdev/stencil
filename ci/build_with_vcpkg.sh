scriptdir = $(dirname $(realpath $0))

export VCPKG_USE_STENCIL_SRC_DIR=${scriptdir}/..

git clone https://github.com/Microsoft/vcpkg.git
git -C vcpkg apply --ignore-space-change --ignore-whitespace $scriptdir/vcpkg/*patch
vcpkg/bootstrap-vcpkg.sh
vcpkg/vcpkg install stencil
