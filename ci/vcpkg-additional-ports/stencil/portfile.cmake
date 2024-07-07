vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ankurvdev/stencil
    REF "9e2ff92d9d2301a6d21a992da5d823d706c5edfd"
    SHA512 2783657d4d5ab8cb719fdb3faeec1745099553edec83f855ee0a719e5758b516e290b9ae78fac0326417ed354fab0eb519a6f8355dc5d471b51ecc41505e6707
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
