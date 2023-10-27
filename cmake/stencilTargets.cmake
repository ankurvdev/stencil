include_guard()
include(FetchContent)

FetchContent_Declare(
    stencil
    GIT_REPOSITORY https://github.com/ankurvdev/stencil.git
    GIT_TAG        v0.1.3
)
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/../src/Thrift.cpp")
    set(STENCIL_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
endif()

function(build_stencil)
    if (PROJECT_NAME STREQUAL stencil OR STENCIL_INSTALL)
        message(FATAL_ERROR "Something is wrong:${STENCIL_SOURCE_DIR}::${PROJECT_NAME}")
    endif()

    if (NOT EXISTS "${STENCIL_SOURCE_DIR}")
        FetchContent_MakeAvailable(stencil)
    endif()

    set(STENCIL_INSTALL OFF CACHE BOOL "Do not install stencil bits")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/stencil-build")
    set(CMD "${CMAKE_COMMAND}" "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/stencil-install")
    if (CMAKE_GENERATOR)
        list(APPEND CMD "-G" "${CMAKE_GENERATOR}")
    endif()

    if (CMAKE_CROSSCOMPILING)
        unset(ENV{CMAKE_CXX_COMPILER})
        unset(ENV{CMAKE_C_COMPILER})
        unset(ENV{CC})
        unset(ENV{CXX})
    endif()


    list(APPEND CMD "${STENCIL_SOURCE_DIR}")

    execute_process(COMMAND ${CMD} WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/stencil-build")
    execute_process(COMMAND "${CMAKE_COMMAND}" --build  "${CMAKE_CURRENT_BINARY_DIR}/stencil-build")
    execute_process(COMMAND "${CMAKE_COMMAND}" --install "${CMAKE_CURRENT_BINARY_DIR}/stencil-build" --prefix "${CMAKE_CURRENT_BINARY_DIR}/stencil-install")
endfunction()

macro(find_or_build_stencil)
    # On Android cross compilation systems avoid the crosscompiled exe
    find_program(STENCIL_EXECUTABLE stencil NO_CMAKE_PATH)
    if (NOT EXISTS "${STENCIL_EXECUTABLE}")
        find_program(STENCIL_EXECUTABLE stencil NO_CACHE)
    endif()
    if (NOT EXISTS "${STENCIL_EXECUTABLE}")
        if ((TARGET stencil) AND (NOT CMAKE_CROSSCOMPILING))
            set(STENCIL_EXECUTABLE stencil)
        elseif((NOT TARGET stencil) AND (NOT CMAKE_CROSSCOMPILING))
            if ((DEFINED VCPKG_ROOT) OR (DEFINED VCPKG_TOOLCHAIN))
                message(FATAL_ERROR "Cannot find_program(stencil). Please install stencil via : vcpkg install stencil")
            endif()
            add_subdirectory("${STENCIL_SOURCE_DIR}" stencil)
        else()
            build_stencil()
            find_program(STENCIL_EXECUTABLE REQUIRED NAMES stencil PATHS "${CMAKE_CURRENT_BINARY_DIR}/stencil-install/bin")
        endif()
    endif()
endmacro()

if (NOT TARGET stencil_runtime)
    add_library(stencil_runtime INTERFACE)
    target_link_libraries(stencil_runtime INTERFACE)
    find_package(stduuid QUIET)
    find_package(fmt QUIET)
    find_package(RapidJSON QUIET)
    find_package(date REQUIRED QUIET)
    # Optional
    find_package(httplib QUIET)

    if (TARGET stduuid)
        target_link_libraries(stencil_runtime INTERFACE stduuid)
        target_compile_definitions(stencil_runtime INTERFACE HAVE_STDUUID=1)
    endif()
    if (TARGET fmt::fmt-header-only)
        target_link_libraries(stencil_runtime INTERFACE fmt::fmt-header-only)
        target_compile_definitions(stencil_runtime INTERFACE HAVE_FMTLIB=1)
    endif()
    if (TARGET rapidjson)
        target_link_libraries(stencil_runtime INTERFACE rapidjson)
        target_compile_definitions(stencil_runtime INTERFACE HAVE_RAPIDJSON=1)
    else()
        message(FATAL_ERROR "Cannot find rapidjson")
    endif()
    if (TARGET date::date)
        target_link_libraries(stencil_runtime INTERFACE date::date)
        target_compile_definitions(stencil_runtime INTERFACE HAVE_DATELIB=1)
    else()
        message(FATAL_ERROR "Cannot find date")
    endif()
    if (TARGET httplib::httplib)
        target_link_libraries(stencil_runtime INTERFACE httplib::httplib)
    endif()

    if (NOT EXISTS "${stencil_INCLUDE_PATH}/stencil/stencil.h")
        find_path(stencil_INCLUDE_PATH "stencil/stencil.h" REQUIRED)
        #set(stencil_INCLUDE_PATH "${stencil_INCLUDE_PATH}" CACHE PATH FORCE "Stencil include path")
    endif()
    file(GLOB stencil_runtime_INCLUDE_FILES "${stencil_INCLUDE_PATH}/stencil/*.h")
    target_sources(stencil_runtime INTERFACE ${stencil_runtime_INCLUDE_FILES})
    target_include_directories(stencil_runtime INTERFACE "${stencil_INCLUDE_PATH}")
endif()

if (NOT TARGET stencil::runtime)
    add_library(stencil::runtime ALIAS stencil_runtime)
endif()

function(_add_stencil_target target libkind)
    cmake_parse_arguments("" "" "" "IDLS" ${ARGN})
    set(outdir "${CMAKE_CURRENT_BINARY_DIR}/stencil_${target}")
    file(MAKE_DIRECTORY ${outdir})
    set(outputs)
    if(TARGET "${STENCIL_EXECUTABLE}")
        foreach(f ${_IDLS})
            get_filename_component(fname "${f}" NAME)
            list(APPEND outputs "${outdir}/${fname}.h")
        endforeach()
    elseif (EXISTS "${STENCIL_EXECUTABLE}")
        execute_process(
            COMMAND "${STENCIL_EXECUTABLE}" --dryrun --outdir=${outdir} ${_IDLS}
            COMMAND_ECHO STDOUT
            RESULT_VARIABLE rslt
            OUTPUT_VARIABLE files)
        if (NOT rslt STREQUAL 0)
            message(FATAL_ERROR "${rslt}::${files}")
        endif()
        STRING(REGEX REPLACE "\n" ";" files "${files}" )
        foreach (f ${files})
            file(TO_CMAKE_PATH "${f}" tmp)
            list(APPEND outputs ${tmp})
        endforeach()
    else()
        message(FATAL_ERROR "Cannot find stencil executable or target ${STENCIL_EXECUTABLE}")
    endif()
    list(APPEND outputs "${outdir}/empty.cpp")
    add_custom_command(OUTPUT ${outputs}
                COMMAND ${STENCIL_EXECUTABLE} --outdir=${outdir} ${_IDLS}
                COMMAND "${CMAKE_COMMAND}" -E touch "${outdir}/empty.cpp"
                DEPENDS ${STENCIL_EXECUTABLE} ${_IDLS}
                COMMENT "Generating IDL code :  ${STENCIL_EXECUTABLE} --outdir=${outdir} ${ARGN}"
                VERBATIM)
    add_library(${target} ${libkind} ${outputs} ${_IDLS})
    target_include_directories(${target} PUBLIC $<BUILD_INTERFACE:${outdir}>)
    target_link_libraries(${target} PUBLIC stencil::runtime)
endfunction()

# IDL Compiler
macro(add_stencil_library target libkind)
    find_or_build_stencil()
    _add_stencil_target(${target} ${libkind} ${ARGN})
endmacro()
