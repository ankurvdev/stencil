function(myexec workdir)
    execute_process(
        COMMAND ${ARGN}
        WORKING_DIRECTORY ${workdir}
        RESULT_VARIABLE result
        ERROR_VARIABLE error
        OUTPUT_VARIABLE output)
    if(NOT "${result}" STREQUAL "0")
        message(FATAL_ERROR
            "Error Executing Command : ${ARGN}\n"
            "WorkDir: ${workdir}\n"
            "Result: ${result} \n"
            "Error: ${error} \n"
            "Output: ${output}")
    endif()
endfunction()

function(target_add_resource target name)
    find_program(embedresource_BINARY embedresource HINTS ${PROJECT_BINARY_DIR})
    if (NOT EXISTS ${embedresource_BINARY})
        set(toolbuilddir ${PROJECT_BINARY_DIR}/buildtool_embedresource)
        set(cmakefile ${toolbuilddir}/CMakeLists.txt)
        file(MAKE_DIRECTORY ${toolbuilddir})
        file(WRITE ${cmakefile} "cmake_minimum_required(VERSION 3.10)
        project(embedresource)
        set(CMAKE_CXX_STANDARD 17)
        add_executable(embedresource ${EmbeddedResource_DIR}/embedresource.cpp)
        install(TARGETS embedresource DESTINATION .)
        ")
        set(CMD ${CMAKE_COMMAND})
        list(APPEND CMD -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR})
        list(APPEND CMD ".")
        myexec(${toolbuilddir} ${CMD})
        myexec(${toolbuilddir} ${CMAKE_COMMAND} --build . -j --target install)
    endif()
    find_program(embedresource_BINARY embedresource HINTS ${PROJECT_BINARY_DIR})
    if (NOT EXISTS ${embedresource_BINARY})
        message(FATAL_ERROR "Cannot find or build embed resource")
    endif()
    set(outdir ${PROJECT_BINARY_DIR}/${target}_embedresource)
    file(MAKE_DIRECTORY ${outdir})
    set(out_f ${outdir}/${name}.cpp)
    if ("${ARGN}" STREQUAL "")
        message(FATAL_ERROR "No args supplied to embed_Resource")
    endif()
    add_custom_command(OUTPUT ${out_f}
        COMMAND ${embedresource_BINARY} ${out_f} ${ARGN}
        DEPENDS ${embedresource_BINARY} ${ARGN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMENT "Building binary file for embedding ${out_f}" VERBATIM)

    target_sources(${target} PRIVATE ${out_f})
    #source_group(TREE . PREFIX Resources FILES ${ARGN})
    #source_group(TREE . PREFIX Generated FILES ${out_f})
    target_include_directories(${target} PRIVATE ${EmbeddedResource_DIR})
endfunction()
