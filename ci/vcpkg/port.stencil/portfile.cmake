if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/use_source_path)
    file(READ ${CMAKE_CURRENT_LIST_DIR}/use_source_path srcpath)
    string(STRIP ${srcpath} srcpath)
    get_filename_component(VCPKG_USE_STENCIL_SRC_DIR ${srcpath} ABSOLUTE)
    set(SOURCE_PATH ${srcpath})
elseif (EXISTS ${CMAKE_CURRENT_LIST_DIR}/use_git)
    file(READ ${CMAKE_CURRENT_LIST_DIR}/use_git gitinfo)
    string(STRIP "${gitinfo}" gitinfo)
    list(GET gitinfo 0 giturl)
    list(GET gitinfo 1 commitId)
    vcpkg_from_git(
        OUT_SOURCE_PATH SOURCE_PATH
        URL ${giturl}
        REF ${commitId}
    )
else()
    set(commitId 8aee47a4d445bf4e5dba4f3bda5ab880b406b693)
    set(sha512 2aba39ad8e667a6373449a951c16fc042ef475cc969f43ccc676bdc6436eb11e0d71d2d8f6b32d4588a168a0b8cbed5733da1e90d740338234eb17050ee83057)
    vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO ankurverma85/stencil
        REF ${commitId}
        SHA512 ${sha512}
        HEAD_REF master)
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

