find_package(Catch2 CONFIG QUIET)
if (NOT TARGET Catch2::Catch2)
    if (COMMAND init_submodule)
        add_subdirectory(${INIT_SUBMODULE_DIRECTORY}/Catch2 Catch2)
        SupressWarningForTarget(Catch2)
        SupressWarningForTarget(Catch2WithMain)
    elseif (EXISTS "${VCPKG_ROOT}" AND DEFINED VCPKG_TARGET_TRIPLET)
        message(STATUS "Attempting to install vcpkg port Catch2:${VCPKG_TARGET_TRIPLET}")
        set(ENV{VCPKG_OVERLAY_PORTS} "${CMAKE_CURRENT_LIST_DIR}/../ci/vcpkg-additional-ports")
        find_program(VCPKG_EXECUTABLE vcpkg REQUIRED PATHS "${VCPKG_ROOT}")
        execute_process(COMMAND "${VCPKG_EXECUTABLE}" install Catch2:${VCPKG_TARGET_TRIPLET} 
            COMMAND_ERROR_IS_FATAL ANY
            WORKING_DIRECTORY "${VCPKG_ROOT}"
        )
        find_package(Catch2 CONFIG QUIET REQUIRED)
    else()
        message(FATAL_ERROR "Cannot find Catch2")
    endif()
endif()
