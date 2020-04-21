set(EmbedResource_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Path for EmbedResource")

function(find_or_build)
    if((EXISTS ${EMBEDRESOURCE_EXECUTABLE}) AND (EXISTS ${EMBEDRESOURCE_INCLUDE_DIR}))
        return()
    endif()

    find_program(binfile embedresource)
    find_file(header EmbeddedResource.h)

    if((EXISTS ${binfile}) AND (EXISTS ${header}))
        get_filename_component(headerdir ${header} DIRECTORY)
        set(EMBEDRESOURCE_EXECUTABLE ${binfile} CACHE PATH "Embed Resource Binary" FORCE)
        set(EMBEDRESOURCE_INCLUDE_DIR ${headerdir} CACHE PATH "Embed Resource Include Dir" FORCE)
        return()
    endif()

    if (NOT EXISTS ${EmbedResource_DIR}/embedresource.cpp)
        message(FATAL_ERROR "Cannot find embedresource code at  ${EmbedResource_DIR}")
    endif()

    if (NOT EXISTS ${EmbedResource_DIR}/EmbeddedResource.h)
        message(FATAL_ERROR "Cannot find embedresource header at  ${EmbedResource_DIR}")
    endif()

    set(EMBEDRESOURCE_INCLUDE_DIR ${EmbedResource_DIR} CACHE PATH "Embed Resource Include Dir" FORCE)

    if (NOT TARGET embedresource)
        add_executable(embedresource ${EmbedResource_DIR}/embedresource.cpp)
        set_target_properties(embedresource PROPERTIES PUBLIC_HEADER "${EmbedResource_DIR}/EmbeddedResource.h")
    endif()
endfunction()

function(target_add_resource target name)
    find_or_build()
    set(outdir ${CMAKE_CURRENT_BINARY_DIR})
    set(out_f ${outdir}/${name}.cpp)

    if ("${ARGN}" STREQUAL "")
        message(FATAL_ERROR "No args supplied to embed_Resource")
    endif()

    foreach (f ${ARGN})
        get_filename_component(tmp ${f} ABSOLUTE)
        list(APPEND inputs ${tmp})
    endforeach()

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
    target_include_directories(${target} PRIVATE ${EMBEDRESOURCE_INCLUDE_DIR})
endfunction()
