vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ankurvdev/stencil
    REF "b73be4dd0c4b96c7e8448e3fce48e36d4aebab8c"
    SHA512 82b88b2ee4fbe0073d2101484c6256f626346d25ae3103a894dd845a0868140a94ef665655ed594cc2d469b9d06d0c380c4f205858362d1f24ac71f2c56afecc
    HEAD_REF main)

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DBUILD_TESTING=OFF
)

vcpkg_cmake_install()
vcpkg_copy_pdbs()

# Handle copyright
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")
if(HOST_TRIPLET STREQUAL TARGET_TRIPLET) # Otherwise fails on wasm32-emscripten
    vcpkg_copy_tools(TOOL_NAMES stencil AUTO_CLEAN)
else()
    file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/bin")
endif()

file(READ "${CURRENT_PACKAGES_DIR}/share/stencil/stencilConfig.cmake" config_contents)
file(WRITE "${CURRENT_PACKAGES_DIR}/share/stencil/stencilConfig.cmake" "find_program(
    stencil_EXECUTABLE stencil
    PATHS
        \"\${CMAKE_CURRENT_LIST_DIR}/../../../${HOST_TRIPLET}/tools/${PORT}\"
    NO_DEFAULT_PATH
    REQUIRED)
${config_contents}"
)
