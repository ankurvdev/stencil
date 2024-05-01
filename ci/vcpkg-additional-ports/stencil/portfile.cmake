vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ankurvdev/stencil
    REF "2ae7f2e4352c64d404aba61d828cefaaca1b60d8"
    SHA512 e8fde40cbb01563325bce0eb8ac1148640db45ebb6f7f217dad1d186cb4429c5d46757c904a963daf6d44343f3b0176fb268b6e5fba584a478d95b615081af0f
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
