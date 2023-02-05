# sha512sum can be obtained using
# wget -O - -q  https://github.com/ankurdev/stencil/archive/b6429f8b92947273a5e66d5f10210b960616a89d.tar.gz | sha512sum
set(commitId f95d44e327ec3fc9dc0a7e4ddeaa880760c789a8)
set(sha512 3b11fdfe93cd07b9e26f71a67fbf28f31ef6ab1623abbe9213bea18ca2f4b4d56fe32ef8340029bfe342b3a8ccfdc02b1b81789cd62fdd22eb9170db71963aa1)
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

