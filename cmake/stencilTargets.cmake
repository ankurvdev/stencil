include(FetchContent)

if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/EmbedResourceConfig.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/EmbedResourceConfig.cmake)
endif()
if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/LexYaccConfig.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/LexYaccConfig.cmake)
endif()

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

FetchContent_Declare(
    STENCIL
    GIT_REPOSITORY https://github.com/ankurvdev/stencil.git
    GIT_TAG        6aedc08492efade9029cdd083c4cf5bcc692cc78
)

function(build_stencil_impl workdir)
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/../src/Thrift.cpp")
        set(STENCIL_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
    else()
        FetchContent_MakeAvailable(STENCIL)
    endif()
    set(builddir ${workdir}/build)
    set(installdir ${workdir})
    file(MAKE_DIRECTORY ${builddir})
    file(MAKE_DIRECTORY ${installdir})

    unset(ENV{CMAKE_CXX_COMPILER})
    unset(ENV{CMAKE_C_COMPILER})
    unset(ENV{CC})
    unset(ENV{CXX})

    set(CMD "${CMAKE_COMMAND}" "-DCMAKE_INSTALL_PREFIX:PATH=${installdir}")
    if (CMAKE_GENERATOR)
        list(APPEND CMD "-G" "${CMAKE_GENERATOR}")
    endif()
    list(APPEND CMD -DBUILD_TESTING=OFF)
    list(APPEND CMD "${STENCIL_SOURCE_DIR}")
    
    execute_process(COMMAND_ERROR_IS_FATAL ANY WORKING_DIRECTORY ${builddir} COMMAND ${CMD})
    execute_process(COMMAND_ERROR_IS_FATAL ANY COMMAND ${CMAKE_COMMAND} --build ${builddir} -j )
    execute_process(COMMAND_ERROR_IS_FATAL ANY COMMAND ${CMAKE_COMMAND} --install ${builddir} -j --prefix "${installdir}")
endfunction()

macro(build_stencil workdir)
    set(stencil_build_dir "${workdir}/buildtool_stencil")
    build_stencil_impl(${stencil_build_dir})
    find_program(STENCIL_EXECUTABLE REQUIRED NAMES stencil HINTS
        ${stencil_build_dir}
        ${stencil_build_dir}/bin
    )
endmacro()

function(_add_stencil_target)
    cmake_parse_arguments("" "" "TARGET" "IDLS" ${ARGN})
    set(targetName ${_TARGET})
    set(outdir "${CMAKE_CURRENT_BINARY_DIR}/stencil_${targetName}")
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

    add_custom_command(OUTPUT ${outputs}
               COMMAND ${STENCIL_EXECUTABLE} --outdir=${outdir} ${_IDLS}
               DEPENDS ${STENCIL_EXECUTABLE} ${_IDLS}
               COMMENT "Generating IDL code :  ${STENCIL_EXECUTABLE} --outdir=${outdir} ${ARGN}"
               VERBATIM)
    add_library(${targetName} INTERFACE ${outputs})
    target_sources(${targetName} INTERFACE ${outputs} ${_IDLS})
    target_include_directories(${targetName} INTERFACE $<BUILD_INTERFACE:${outdir}>)
    target_link_libraries(${targetName} INTERFACE stencil::runtime)
endfunction()

# IDL Compiler
macro(add_stencil_library)
    if (NOT EXISTS "${STENCIL_EXECUTABLE}")
        find_program(STENCIL_EXECUTABLE stencil)
        if (NOT EXISTS "${STENCIL_EXECUTABLE}")
            if (CMAKE_CROSSCOMPILING)
                build_stencil(${CMAKE_CURRENT_BINARY_DIR})
            else()
                if (TARGET stencil)
                    unset(STENCIL_EXECUTABLE CACHE)
                    set(STENCIL_EXECUTABLE stencil CACHE STRING "Stencil Executable")
                else()
                    build_stencil(${CMAKE_CURRENT_BINARY_DIR})
                endif()
            endif()
        endif()
    endif()

    if (NOT EXISTS "${STENCIL_EXECUTABLE}" AND NOT TARGET stencil)
        message(FATAL_ERROR "Cannot find or build stencil")
    endif()
    
    _add_stencil_target(${ARGN})
endmacro()
