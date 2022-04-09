set(EmbedResource_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Path for EmbedResource")

function(build_embedresource)
    if (TARGET embedresource)
        return()
    endif()
    add_executable(embedresource ${EmbedResource_DIR}/embedresource.cpp)
    set_target_properties(embedresource PROPERTIES PUBLIC_HEADER "${EmbedResource_DIR}/EmbeddedResource.h")
    set(EMBEDRESOURCE_INCLUDE_DIR ${EmbedResource_DIR} CACHE PATH "Embed Resource Include Dir")
endfunction()

function(find_or_build)
    if (TARGET embedresource)
        return()
    endif()

    if((EXISTS ${EMBEDRESOURCE_EXECUTABLE}) AND (EXISTS ${EMBEDRESOURCE_INCLUDE_DIR}))
        return()
    endif()

    find_program(EMBEDRESOURCE_EXECUTABLE embedresource)
    find_file(header EmbeddedResource.h HINTS ${EmbedResource_DIR}/../../../include)

    if((EXISTS ${EMBEDRESOURCE_EXECUTABLE}) AND (EXISTS ${header}))
        get_filename_component(headerdir ${header} DIRECTORY)
        set(EMBEDRESOURCE_INCLUDE_DIR ${headerdir} CACHE PATH "Embed Resource Include Dir")
        return()
    endif()

    if (NOT EXISTS ${EmbedResource_DIR}/embedresource.cpp)
        message(FATAL_ERROR "Cannot find embedresource code at  ${EmbedResource_DIR}")
    endif()

    if (NOT EXISTS ${EmbedResource_DIR}/EmbeddedResource.h)
        message(FATAL_ERROR "Cannot find embedresource header at  ${EmbedResource_DIR}")
    endif()

    set(EMBEDRESOURCE_INCLUDE_DIR ${EmbedResource_DIR} CACHE PATH "Embed Resource Include Dir")

    build_embedresource()
endfunction()

function(target_add_resource target name)
    find_or_build()
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
    if (TARGET embedresource)
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

function(add_resource_library)
    cmake_parse_arguments("" "" "TARGET" "RESOURCES" ${ARGN})
    add_library(${_TARGET} OBJECT)
    target_add_resource(${_TARGET} ${_TARGET} ${_RESOURCES})
endfunction()