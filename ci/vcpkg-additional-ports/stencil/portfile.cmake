# sha512sum can be obtained using
# wget -O - -q  https://github.com/ankurdev/stencil/archive/b6429f8b92947273a5e66d5f10210b960616a89d.tar.gz | sha512sum
set(commitId 57786db7cbc56a0c5588045464c28831f386779e)
set(sha512 a5d349aa5943fde805fe947b076cd977a0b31529212a82bece66496b8ae91f2fd625e83fdeaa2e453aad73720a16b4a21402ddb89507d4e76df5e326f519a4b1)
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ankurvdev/stencil
    REF ${commitId}
    SHA512 ${sha512}
    HEAD_REF master)

vcpkg_find_acquire_program(FLEX)
vcpkg_find_acquire_program(BISON)

if(NOT TARGET_TRIPLET STREQUAL HOST_TRIPLET)
    vcpkg_add_to_path(PREPEND "${CURRENT_HOST_INSTALLED_DIR}/tools")
endif()

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        -DSTENCIL_INSTALL_BUILDTOOLS=ON
        -DBUILD_TESTING=OFF
        -DBISON_EXECUTABLE=${BISON}
        -DFLEX_EXECUTABLE=${FLEX}
)

vcpkg_install_cmake()
vcpkg_copy_pdbs()

# Handle copyright
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")
file(RENAME "${CURRENT_PACKAGES_DIR}/bin" "${CURRENT_PACKAGES_DIR}/tools")

