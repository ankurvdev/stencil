scriptdir = $(dirname $(realpath $0))
if [[ "$1" == "" ]]; then 
    $triplet = "x64-linux"
else 
    $triplet = "$1"
fi

export VCPKG_USE_STENCIL_SRC_DIR=${scriptdir}/..

git clone https://github.com/Microsoft/vcpkg.git
git apply --ignore-space-change --ignore-whitespace $scriptdir/vcpkg/*patch
vcpkg/bootstrap-vcpkg.sh
vcpkg/vcpkg install stencil:$triplet
