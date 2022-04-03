vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO mariusbancila/stduuid
    REF 5c538cca02932aa0266659661d5b4726f3a317c7
    SHA512 d83571c17726e484dad7ea4dc365b79013a24db50de35fcaca2bcf7206ced395fd8d0ad936d749298bf6afb6ad5aabc6e91c2c22ba60a2a7afb7af501b89340d
    HEAD_REF master)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        -DUUID_BUILD_TESTS:BOOL=OFF
)

vcpkg_install_cmake()
vcpkg_copy_pdbs()

# Handle copyright
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")
file(RENAME "${CURRENT_PACKAGES_DIR}/lib/cmake/stduuid" "${CURRENT_PACKAGES_DIR}/share/${PORT}")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/lib")
