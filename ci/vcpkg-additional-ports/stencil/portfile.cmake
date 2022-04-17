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
    # wget -O - -q  https://github.com/ankurverma85/stencil/archive/c12fc097999c674debd69270ad491a9d43a50ae3.tar.gz | sha512sum 
    set(commitId 3f7d6013ccdc857cbd9d9d9622a5bc99114fb908)
    set(sha512 69df9bd4c9df6ff5ab82fb40c904b284ccf974033bbeb70a0c99b19ded353d53e8d5e6bd576bc24e326779bd791c903207ae416584754fd748e48a1d76305ba4) 
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

