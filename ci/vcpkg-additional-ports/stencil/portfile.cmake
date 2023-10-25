vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ankurvdev/stencil
    REF "v${VERSION}"
    SHA512 a5d349aa5943fde805fe947b076cd977a0b31529212a82bece66496b8ae91f2fd625e83fdeaa2e453aad73720a16b4a21402ddb89507d4e76df5e326f519a4b1
    HEAD_REF main)

vcpkg_find_acquire_program(FLEX)
vcpkg_find_acquire_program(BISON)
    
vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
    -DSTENCIL_INSTALL_BUILDTOOLS=ON
    -DBUILD_TESTING=OFF
    -DBISON_EXECUTABLE=${BISON}
    -DFLEX_EXECUTABLE=${FLEX}
)

vcpkg_cmake_install()
vcpkg_copy_pdbs()

# Handle copyright
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")
if(HOST_TRIPLET STREQUAL TARGET_TRIPLET)
    vcpkg_copy_tools(TOOL_NAMES stencil AUTO_CLEAN)
else()
    message(FATAL_ERROR "Skipping stencil installation for ${HOST_TRIPLET} != ${TARGET_TRIPLET}")
endif()

file(READ "${CURRENT_PACKAGES_DIR}/share/stencil/stencilConfig.cmake" config_contents)
file(WRITE "${CURRENT_PACKAGES_DIR}/share/stencil/stencilConfig.cmake"
"find_program(
    stencil_EXECUTABLE stencil
    PATHS
        \"\${CMAKE_CURRENT_LIST_DIR}/../../../${HOST_TRIPLET}/tools/${PORT}\"
    NO_DEFAULT_PATH
    REQUIRED)
${config_contents}"
)
