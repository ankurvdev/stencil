include_guard()
include(FetchContent)
include(${CMAKE_CURRENT_LIST_DIR}/FindOrBuildTool.cmake)

FetchContent_Declare(
    stencil
    GIT_REPOSITORY https://github.com/ankurvdev/stencil.git
    GIT_TAG        main
    GIT_SHALLOW 1
    SYSTEM
)
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/../src/Thrift.cpp")
    set(stencil_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
endif()

if (NOT TARGET stencil_runtime)
    add_library(stencil_runtime INTERFACE)
    target_link_libraries(stencil_runtime INTERFACE)

    if (NOT EXISTS "${stencil_INCLUDE_PATH}/stencil/stencil.h")
        find_path(stencil_INCLUDE_PATH "stencil/stencil.h" REQUIRED)
        #set(stencil_INCLUDE_PATH "${stencil_INCLUDE_PATH}" CACHE PATH FORCE "Stencil include path")
    endif()
    file(GLOB stencil_runtime_INCLUDE_FILES "${stencil_INCLUDE_PATH}/stencil/*.h")
    target_sources(stencil_runtime INTERFACE ${stencil_runtime_INCLUDE_FILES})
    target_include_directories(stencil_runtime INTERFACE "${stencil_INCLUDE_PATH}")
endif()

find_package(stduuid QUIET)
find_package(fmt QUIET)
find_package(RapidJSON QUIET)
find_package(date REQUIRED QUIET)
# Optional
find_package(httplib QUIET)
find_package(Boost QUIET COMPONENTS beast url) # beast is header only. Components are for non-header libs

if (TARGET stduuid)
    target_link_libraries(stencil_runtime INTERFACE stduuid)
    target_compile_definitions(stencil_runtime INTERFACE HAVE_STDUUID=1)
endif()
if (TARGET fmt::fmt-header-only)
    target_link_libraries(stencil_runtime INTERFACE  fmt::fmt-header-only)
    target_compile_definitions(stencil_runtime INTERFACE HAVE_FMTLIB=1)
endif()
if (TARGET rapidjson)
    target_link_libraries(stencil_runtime INTERFACE rapidjson)
    target_compile_definitions(stencil_runtime INTERFACE HAVE_RAPIDJSON=1)
else()
    message(FATAL_ERROR "Cannot find rapidjson")
endif()
if (TARGET date::date)
    target_link_libraries(stencil_runtime INTERFACE  date::date)
    target_compile_definitions(stencil_runtime INTERFACE HAVE_DATELIB=1)
else()
    message(FATAL_ERROR "Cannot find date")
endif()
if (TARGET httplib::httplib)
    target_link_libraries(stencil_runtime INTERFACE  httplib::httplib)
endif()
if (TARGET Boost::beast)
    target_compile_definitions(stencil_runtime INTERFACE HAVE_BOOSTBEAST=1)
    target_link_libraries(stencil_runtime INTERFACE Boost::beast Boost::url)
    if (MINGW)
        target_link_libraries(stencil_runtime INTERFACE ws2_32 mswsock)
    endif()
else()
    message(FATAL_ERROR "Cannot find Boost::beast")
endif()

if (NOT TARGET stencil::runtime)
    add_library(stencil::runtime ALIAS stencil_runtime)
endif()

function(_add_stencil_target target libkind)
    cmake_parse_arguments("" "" "" "IDLS" ${ARGN})
    set(outdir "${CMAKE_CURRENT_BINARY_DIR}/stencil_${target}")
    file(MAKE_DIRECTORY ${outdir})
    set(outputs)
    if(TARGET "${stencil_EXECUTABLE}")
        foreach(f ${_IDLS})
            get_filename_component(fname "${f}" NAME)
            list(APPEND outputs "${outdir}/${fname}.h")
        endforeach()
    elseif (EXISTS "${stencil_EXECUTABLE}")
        execute_process(
            COMMAND "${stencil_EXECUTABLE}" --dryrun --outdir=${outdir} ${_IDLS}
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
        message(FATAL_ERROR "Cannot find stencil executable or target ${stencil_EXECUTABLE}")
    endif()
    list(APPEND outputs "${outdir}/empty.cpp")
    add_custom_command(OUTPUT ${outputs}
                COMMAND ${stencil_EXECUTABLE} --outdir=${outdir} ${_IDLS}
                COMMAND "${CMAKE_COMMAND}" -E touch "${outdir}/empty.cpp"
                DEPENDS ${stencil_EXECUTABLE} ${_IDLS}
                COMMENT "Generating IDL code :  ${stencil_EXECUTABLE} --outdir=${outdir} ${ARGN}"
                VERBATIM)
    add_library(${target} ${libkind} ${outputs} ${_IDLS})
    target_include_directories(${target} PUBLIC $<BUILD_INTERFACE:${outdir}>)
    target_link_libraries(${target} PUBLIC stencil::runtime)
endfunction()

# IDL Compiler
macro(add_stencil_library target libkind)
    FindOrBuildTool(stencil)
    _add_stencil_target(${target} ${libkind} ${ARGN})
endmacro()
