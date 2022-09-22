# sha512sum can be obtained using
# wget -O - -q  https://github.com/ankurdev/stencil/archive/b6429f8b92947273a5e66d5f10210b960616a89d.tar.gz | sha512sum
set(commitId 924b46e2e846c234ca0e5a2112ba417be0230eba)
set(sha512 3463430c79e70286802486557ae693ce446c6db1def8011bb41b33c545616faf268c1a30512118b0fe2046b876a3243406240a07c4d3aca8f26a8c2719bdf0d8)
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

