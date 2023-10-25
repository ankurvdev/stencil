include_guard()

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        10.1.1
    SOURCE_SUBDIR  .
    GIT_PROGRESS TRUE
    FIND_PACKAGE_ARGS NAMES fmt
)

if (COMMAND vcpkg_install)
    vcpkg_install(fmt)
endif()

FetchContent_MakeAvailable(fmt)
if (COMMAND SupressWarningForTarget)
    SupressWarningForTarget(fmt)
endif()
