find_path(DTL_INCLUDE_DIRS "dtl/Diff.hpp")

if (NOT EXISTS "${DTL_INCLUDE_DIRS}")
    if (COMMAND init_submodule)
        init_submodule(dtl)
        set(DTL_INCLUDE_DIRS "${INIT_SUBMODULE_DIRECTORY}/dtl")
    elseif (EXISTS "${VCPKG_ROOT}" AND DEFINED VCPKG_TARGET_TRIPLET)
        message(STATUS "Attempting to install vcpkg port dtl:${VCPKG_TARGET_TRIPLET}")
        set(ENV{VCPKG_OVERLAY_PORTS} "${CMAKE_CURRENT_LIST_DIR}/../ci/vcpkg-additional-ports")
        find_program(VCPKG_EXECUTABLE vcpkg REQUIRED PATHS "${VCPKG_ROOT}")
        execute_process(COMMAND "${VCPKG_EXECUTABLE}" install dtl:${VCPKG_TARGET_TRIPLET} 
            COMMAND_ERROR_IS_FATAL ANY
            WORKING_DIRECTORY "${VCPKG_ROOT}"
        )
        unset(DTL_INCLUDE_DIRS CACHE)
        find_path(DTL_INCLUDE_DIRS "dtl/Diff.hpp")
    else()
        message(FATAL_ERROR "Cannot find dtl")
    endif()
endif()

add_library(dtl INTERFACE)
add_library(dtl::dtl ALIAS dtl)
target_include_directories(dtl INTERFACE "${DTL_INCLUDE_DIRS}")
