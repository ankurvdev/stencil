if (DEFINED ENV{VCPKG_USE_STENCIL_SRC_DIR} AND EXISTS "$ENV{VCPKG_USE_STENCIL_SRC_DIR}")
    get_filename_component(srcpath "$ENV{VCPKG_USE_STENCIL_SRC_DIR}" ABSOLUTE)
    set(SOURCE_PATH "${srcpath}")
elseif (DEFINED ENV{VCPKG_USE_STENCIL_GIT_REPO} AND NOT "$ENV{VCPKG_USE_STENCIL_GIT_REPO}" STREQUAL "")
    set(gitinfo "$ENV{VCPKG_USE_STENCIL_GIT_REPO}")
    list(GET gitinfo 0 giturl)
    list(GET gitinfo 1 commitId)
    vcpkg_from_git(
        OUT_SOURCE_PATH SOURCE_PATH
        URL ${giturl}
        REF ${commitId}
    )
else()
    set(commitId 2ed8982d6c41a737a238bfe999a37b39dd53e3d5)
    set(sha512 6c36d1777b8f0edb870b07e444847f898a79707ab904a3062880d36afae1de72d9dc1b0f6db6a2a59bbbb984a772cb97cb6d6728c4db2b40aa70f29697a7b5f7)
    vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO ankurverma85/stencil
        REF ${commitId}
        SHA512 ${sha512}
        HEAD_REF master)
endif()
# Dont use these these are older version
#vcpkg_find_acquire_program(FLEX)
#vcpkg_find_acquire_program(BISON)
if(NOT TARGET_TRIPLET STREQUAL HOST_TRIPLET)
    vcpkg_add_to_path(PREPEND "${CURRENT_HOST_INSTALLED_DIR}/tools")
endif()

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        -DSTENCIL_INSTALL_BUILDTOOLS=ON
        -DBUILD_TESTING=OFF
)

vcpkg_install_cmake()
vcpkg_copy_pdbs()

# Handle copyright
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")
file(RENAME "${CURRENT_PACKAGES_DIR}/bin" "${CURRENT_PACKAGES_DIR}/tools")

