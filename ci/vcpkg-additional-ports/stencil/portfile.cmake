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
    # sha512sum can be obtained using
    # wget -O - -q  https://github.com/ankurverma85/stencil/archive/b6429f8b92947273a5e66d5f10210b960616a89d.tar.gz | sha512sum 
    set(commitId 940eb82e7c560123e9d5aa92cb5670dd087918fb)
    set(sha512 ced9fa9822298c6934d2e303f2695415881b9bec619a9cc8cd3338aa11bdb3b04a0ed4e9d4c29c979fe83568cc0b876fdc1342244aaff43f156091701a5fa575)
    vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO ankurvdev/stencil
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

