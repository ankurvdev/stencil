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
    set(commitId 7cd3e15b10773cd60b6f28ccc6f1389d4141f64e)
    set(sha512 252528cf1b165d39dc1f77d7d289bdb394697ad5f485adf4755fbd8e0fa5a0467423a8a1e072185b067b86e856024bac1472ffca2ba4764abdea200d3d78b38d)
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
    OPTIONS -DSTENCIL_INSTALL_BUILDTOOLS=ON
)

vcpkg_install_cmake()
vcpkg_copy_pdbs()

# Handle copyright
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")
file(RENAME "${CURRENT_PACKAGES_DIR}/bin" "${CURRENT_PACKAGES_DIR}/tools")

