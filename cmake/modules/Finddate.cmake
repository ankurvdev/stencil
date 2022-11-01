find_package(date CONFIG QUIET)
if (NOT TARGET date::date)
    if (COMMAND init_submodule)
        init_submodule(date)
        add_subdirectory(${INIT_SUBMODULE_DIRECTORY}/date EXCLUDE_FROM_ALL date)
        #SupressWarningForTarget(date)
    elseif (EXISTS "${VCPKG_ROOT}" AND DEFINED VCPKG_TARGET_TRIPLET)
        message(STATUS "Attempting to install vcpkg port fmt:${VCPKG_TARGET_TRIPLET}")
        set(ENV{VCPKG_OVERLAY_PORTS} "${CMAKE_CURRENT_LIST_DIR}/../ci/vcpkg-additional-ports")
        find_program(VCPKG_EXECUTABLE vcpkg REQUIRED PATHS ${VCPKG_ROOT})
        execute_process(COMMAND "${VCPKG_EXECUTABLE}" install date:${VCPKG_TARGET_TRIPLET} 
            COMMAND_ERROR_IS_FATAL ANY
            WORKING_DIRECTORY "${VCPKG_ROOT}"
        )
        find_package(date CONFIG REQUIRED)
    else()
        message(FATAL_ERROR "Cannot find date")
    endif()
endif()
