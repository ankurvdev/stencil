# sha512sum can be obtained using
# wget -O - -q  https://github.com/ankurdev/stencil/archive/b6429f8b92947273a5e66d5f10210b960616a89d.tar.gz | sha512sum
set(commitId 9f033f4d534c5d391e332697f026004f222c5d86)
set(sha512 a02173739073a45df05b634ff0cd9740d02a4fcf59470e83eeb977b89e4d4c9ac5c15262bb502162713f8205c069db73a8017d5a64dc4ff823ae84c8b680db83)
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

