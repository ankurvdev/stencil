vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ankurvdev/stencil
    REF "2ae7f2e4352c64d404aba61d828cefaaca1b60d8"
    SHA512 8ddec1eee4ecd1ac8643f9f6b55e6510e7d5bd4b3f473c6a10cec38fba46c2bd7163d4644d0a48e11756927ba27f2ccf871609478f1dcddd72e984e1c770a074
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
