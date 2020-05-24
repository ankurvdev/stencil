if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/EmbedResourceConfig.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/EmbedResourceConfig.cmake)
endif()
if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/LexYaccConfig.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/LexYaccConfig.cmake)
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

    set(STENCIL_EXECUTABLE ${binfile} CACHE PATH "Location of the stencil binary to use" FORCE)
endfunction()

# IDL Compiler
function(target_add_stencil target)
    if (NOT EXISTS ${STENCIL_EXECUTABLE})
        find_program(STENCIL_EXECUTABLE stencil)
    endif()

    if (NOT EXISTS ${STENCIL_EXECUTABLE})
        build_stencil()
    endif()

    if (NOT EXISTS ${STENCIL_EXECUTABLE})
        message(FATAL_ERROR "Cannot find or build stencil")
    endif()

    foreach (f ${ARGN})
        file(TO_CMAKE_PATH "${f}" tmp)
        list(APPEND inputs ${tmp})
    endforeach()

    get_filename_component(stencil_dir ${STENCIL_EXECUTABLE} DIRECTORY)

    set(outputs)

    set(outdir ${CMAKE_CURRENT_BINARY_DIR}/stencil_${target})
    file(MAKE_DIRECTORY ${outdir})

    execute_process(
        COMMAND ${STENCIL_EXECUTABLE} --dryrun --outdir=${outdir} ${inputs}
        RESULT_VARIABLE result
        ERROR_VARIABLE error
        OUTPUT_VARIABLE files)

    if(NOT result STREQUAL 0)
        list(JOIN ARGN " " argslist)
        message(FATAL_ERROR
        "Error Executing Command"
        "${STENCIL_EXECUTABLE} --dryrun --outdir=${outdir} ${argslist}"
        "WorkDir: ${workdir}\n"
        "Result: ${result} \n"
        "Error: ${error} \n"
        "Output: ${output}")
    endif()

    STRING(REGEX REPLACE "\n" ";" files "${files}" )
    foreach (f ${files})
        file(TO_CMAKE_PATH "${f}" tmp)
        list(APPEND outputs ${tmp})
    endforeach()

    add_custom_command(OUTPUT ${outputs}
               COMMAND ${STENCIL_EXECUTABLE} --outdir=${outdir} ${ARGN}
               DEPENDS ${STENCIL_EXECUTABLE} ${inputs}
               COMMENT "Generating IDL code :  ${STENCIL_EXECUTABLE} --outdir=${outdir} ${ARGN}"
               VERBATIM)

    target_include_directories(${target} PUBLIC ${stencil_dir}/../include)
    target_include_directories(${target} PUBLIC ${outdir} )
    target_sources(${target} PRIVATE ${inputs} ${outputs})
endfunction()
