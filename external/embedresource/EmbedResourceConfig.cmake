set(EmbedResource_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Path for EmbedResource")

function(build_embedresource)
    if (TARGET embedresource)
        return()
    endif()
    add_executable(embedresource ${EmbedResource_DIR}/embedresource.cpp)
    set_target_properties(embedresource PROPERTIES PUBLIC_HEADER "${EmbedResource_DIR}/EmbeddedResource.h")
    set(EMBEDRESOURCE_INCLUDE_DIR ${EmbedResource_DIR} CACHE PATH "Embed Resource Include Dir")
endfunction()

macro(target_add_resource target name)
    find_program(EMBEDRESOURCE_EXECUTABLE embedresource)
    find_path(EMBEDRESOURCE_INCLUDE_DIR EmbeddedResource.h HINTS
        "${EmbedResource_DIR}/../../../include"
    )
    if (NOT EXISTS "${EMBEDRESOURCE_EXECUTABLE}")
        build_embedresource()
    endif()
    _target_add_resource(${target} ${name} ${ARGN})
endmacro()

function(_target_add_resource target name)
    set(outdir ${CMAKE_CURRENT_BINARY_DIR}/ressource_${target})
    set(out_f ${outdir}/${name}.cpp)
    file(MAKE_DIRECTORY "${outdir}")
    if ("${ARGN}" STREQUAL "")
        message(FATAL_ERROR "No args supplied to embed_Resource")
    endif()

    foreach (f ${ARGN})
        get_filename_component(tmp ${f} ABSOLUTE)
        list(APPEND inputs ${tmp})
    endforeach()
    list(REMOVE_DUPLICATES inputs)

    if (NOT EXISTS "${EMBEDRESOURCE_EXECUTABLE}")
        add_custom_command(OUTPUT ${out_f}
            COMMAND embedresource ${out_f} ${inputs}
            DEPENDS embedresource ${inputs}
            COMMENT "Building binary file for embedding ${out_f}")
    else()
        add_custom_command(OUTPUT ${out_f}
            COMMAND ${EMBEDRESOURCE_EXECUTABLE} ${out_f} ${inputs}
            DEPENDS ${EMBEDRESOURCE_EXECUTABLE} ${inputs}
            COMMENT "Building binary file for embedding ${out_f}")
    endif()

    target_sources(${target} PRIVATE ${out_f})
    target_include_directories(${target} PUBLIC "${EMBEDRESOURCE_INCLUDE_DIR}")
    target_compile_definitions(${target} PUBLIC USE_EMBEDRESOURCE)
endfunction()

macro(add_resource_library)
    cmake_parse_arguments("" "" "TARGET" "RESOURCES" ${ARGN})
    add_library(${_TARGET} OBJECT)
    target_add_resource(${_TARGET} ${_TARGET} ${_RESOURCES})
endmacro()