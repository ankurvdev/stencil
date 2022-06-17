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
    find_package(date QUIET)

    if (TARGET stduuid::stduuid)
        target_link_libraries(stencil_runtime INTERFACE stduuid::stduuid)
        target_compile_definitions(stencil_runtime INTERFACE HAS_STDUUID=1)
    endif()
    if (TARGET fmt::fmt-header-only)
        target_link_libraries(stencil_runtime INTERFACE fmt::fmt-header-only)
        target_compile_definitions(stencil_runtime INTERFACE HAS_FMTLIB=1)
    endif()
    if (TARGET rapidjson)
        target_link_libraries(stencil_runtime INTERFACE rapidjson)
        target_compile_definitions(stencil_runtime INTERFACE HAS_RAPIDJSON=1)
    endif()
    if (TARGET date::date)
        target_link_libraries(stencil_runtime INTERFACE date::date)
        target_compile_definitions(stencil_runtime INTERFACE HAS_DATELIB=1)
    else()
        message(FATAL_ERROR "Cannot find date")
    endif()
    add_library(stencil::runtime ALIAS stencil_runtime)
endif()

function(build_stencil workdir)
    set(workdir ${CMAKE_CURRENT_BINARY_DIR}/buildtool_stencil)
    set(srcdir ${workdir}/stencil)
    set(builddir ${workdir}/build)
    set(installdir ${workdir})
    file(MAKE_DIRECTORY ${builddir})
    file(MAKE_DIRECTORY ${installdir})
    file(MAKE_DIRECTORY ${srcdir})

    find_package(Git REQUIRED)
    set(giturl "https://github.com/ankurverma85/stencil")

    execute_process(
        COMMAND ${GIT_EXECUTABLE} clone ${giturl} ${srcdir} 
        RESULT_VARIABLE rslt WORKING_DIRECTORY ${workdir})
    if (NOT rslt STREQUAL 0)
        message(FATAL_ERROR "Unable to clone git repo for building stencil")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} ${srcdir}
        RESULT_VARIABLE rslt WORKING_DIRECTORY ${builddir})
    if (NOT rslt STREQUAL 0)
        message(FATAL_ERROR "Unable to configure stencil ${rslt}")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build . -j --config Debug --target install --prefix ${installdir}
        RESULT_VARIABLE rslt WORKING_DIRECTORY ${builddir})
    if (NOT rslt STREQUAL 0)
        message(FATAL_ERROR "Unable to build stencil ${rslt}")
    endif()

    find_program(binfile stencil HINTS ${installdir})
    if (not exists binfile)
        message(FATAL_ERROR "Unable to find installed binary")
    endif()

    set(STENCIL_EXECUTABLE ${binfile} CACHE PATH "Location of the stencil binary to use")
endfunction()

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
    target_include_directories(${targetName} INTERFACE ${stencil_INCLUDE_PATH})
    target_include_directories(${targetName} INTERFACE ${outdir})
    target_link_libraries(${targetName} INTERFACE stencil::runtime date::date fmt::fmt-header-only)
endfunction()

# IDL Compiler
macro(add_stencil_library)
    if (NOT DEFINED STENCIL_EXECUTABLE)
        find_program(STENCIL_EXECUTABLE stencil)
        if (NOT EXISTS "${STENCIL_EXECUTABLE}")
            if (TARGET stencil)
                unset(STENCIL_EXECUTABLE CACHE)
                set(STENCIL_EXECUTABLE stencil CACHE STRING "Stencil Executable")
            else()
                build_stencil(${CMAKE_CURRENT_BINARY_DIR})
            endif()
        endif()
    endif()
    if (NOT DEFINED STENCIL_EXECUTABLE)
        message(FATAL_ERROR "Cannot find or build stencil")
    endif()
    if (NOT EXISTS "${stencil_INCLUDE_PATH}/stencil/stencil.h")
        find_path(stencil_INCLUDE_PATH "stencil/stencil.h" REQUIRED)
        set(stencil_INCLUDE_PATH "${stencil_INCLUDE_PATH}" CACHE PATH FORCE "Stencil include path")
    endif()
    _add_stencil_target(${ARGN})
endmacro()
