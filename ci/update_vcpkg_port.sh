#!/bin/bash
set -e
set -o pipefail

commitId=$(git rev-parse HEAD)
scriptdir=$(dirname $(readlink -f $0))
portfile=${scriptdir}/vcpkg-additional-ports/stencil/portfile.cmake
sha512=$(wget -O - -q  https://github.com/ankurvdev/stencil/archive/${commitId}.tar.gz | sha512sum -z | cut - -d' ' -f1)

sed -ri "s/set\(commitId [a-f0-9]+\)/set(commitId ${commitId})/g" ${portfile}
sed -ri "s/set\(sha512 [a-f0-9]+\)/set(sha512 ${sha512})/g"       ${portfile}

git diff
git checkout ${commitId}
git -c user.email="ankur.verma@outlook.com" -c  user.name="Ankur Verma" -C ${scriptdir} commit vcpkg-additional-ports/stencil/portfile.cmake -m "Update VCPKG Port to ${commitId}"
git checkout -b vcpkg
git push origin vcpkg --force --set-upstream