include_guard()

FetchContent_Declare(
    dtl
    GIT_REPOSITORY https://github.com/cubicdaiya/dtl
    GIT_TAG v1.20
    SOURCE_SUBDIR .
    GIT_PROGRESS TRUE
    FIND_PACKAGE_ARGS NAMES dtl
)
if (COMMAND vcpkg_install)
    vcpkg_install(dtl)
endif()

find_path(DTL_INCLUDE_DIRS "dtl/Diff.hpp")

if (DTL_INCLUDE_DIRS-NOTFOUND)
    FetchContent_MakeAvailable(dtl)
    find_path(DTL_INCLUDE_DIRS "dtl/Diff.hpp" HINTS "${dtl_SOURCE_DIR}" REQUIRED)
endif()

if (NOT TARGET dtl)
    add_library(dtl INTERFACE)
    add_library(dtl::dtl ALIAS dtl)
    target_include_directories(dtl INTERFACE "${DTL_INCLUDE_DIRS}")
endif()