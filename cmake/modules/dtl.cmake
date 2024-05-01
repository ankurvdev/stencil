include_guard()
include(FetchContent)

FetchContent_Declare(
    dtl
    GIT_REPOSITORY https://github.com/cubicdaiya/dtl
    GIT_TAG v1.20
    SOURCE_SUBDIR .
    GIT_PROGRESS TRUE
    SYSTEM
    FIND_PACKAGE_ARGS NAMES dtl
)
if (COMMAND vcpkg_install)
    vcpkg_install(dtl)
endif()

find_path(DTL_INCLUDE_DIRS "dtl/Diff.hpp")
if ("${DTL_INCLUDE_DIRS}" STREQUAL "DTL_INCLUDE_DIRS-NOTFOUND")
    FetchContent_MakeAvailable(dtl)
    FetchContent_GetProperties(dtl)
    message(STATUS "\n\tdtl_SOURCE_DIR=${dtl_SOURCE_DIR}\n\tdtl_POPULATED=${dtl_POPULATED}\n\tdtl_BINARY_DIR=${dtl_BINARY_DIR}")
    if(NOT dtl_POPULATED)
        FetchContent_Populate(dtl)
    endif()
    message(STATUS "\n\tdtl_SOURCE_DIR=${dtl_SOURCE_DIR}\n\tdtl_POPULATED=${dtl_POPULATED}\n\tdtl_BINARY_DIR=${dtl_BINARY_DIR}")
    find_path(DTL_INCLUDE_DIRS "dtl/Diff.hpp" HINTS "${dtl_SOURCE_DIR}" REQUIRED)
endif()

if (NOT EXISTS "${DTL_INCLUDE_DIRS}/dtl/Diff.hpp")
    message(FATAL_ERROR "dtl not found at ${DTL_INCLUDE_DIRS}/dtl/Diff.hpp")
endif()

if (NOT TARGET dtl)
    add_library(dtl INTERFACE)
    add_library(dtl::dtl ALIAS dtl)
    target_include_directories(dtl INTERFACE "${DTL_INCLUDE_DIRS}")
endif()