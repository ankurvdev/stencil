# sha512sum can be obtained using
# wget -O - -q  https://github.com/ankurdev/stencil/archive/b6429f8b92947273a5e66d5f10210b960616a89d.tar.gz | sha512sum
set(commitId 00d49a92b08259abe14503a3db9d26159fa130f0)
set(sha512 756cfb2c2db11cb16b9a48a51bbd0322cceca504d34c6c792e9c08fa19b8def96eedc34a2b5f3c8a07096e8cf490dffdf306e68b94dd1e3739ec639508373390)
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

