include_guard()
include(FetchContent)

FetchContent_Declare(
    tsl-ordered-map
    GIT_REPOSITORY https://github.com/Tessil/ordered-map
    GIT_TAG v1.1.0
    SOURCE_SUBDIR .
    GIT_PROGRESS TRUE
    FIND_PACKAGE_ARGS NAMES tsl-ordered-map
)

if (COMMAND vcpkg_install)
    vcpkg_install(tsl-ordered-map)
endif()

FetchContent_MakeAvailable(tsl-ordered-map)
