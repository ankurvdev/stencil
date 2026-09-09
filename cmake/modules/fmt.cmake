include_guard()
include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        12.2.0
    SOURCE_SUBDIR  .
    GIT_PROGRESS FALSE GIT_SHALLOW ON SYSTEM
    FIND_PACKAGE_ARGS 12.2.0 NAMES fmt
)

if (COMMAND vcpkg_install)
    vcpkg_install(fmt)
endif()

FetchContent_MakeAvailable(fmt)
if (COMMAND SupressWarningForTarget)
    SupressWarningForTarget(fmt)
    SupressWarningForTarget(fmt-c)
    SupressWarningForTarget(fmt-module)
endif()
