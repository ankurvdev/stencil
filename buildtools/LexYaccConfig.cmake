# Lex and Yacc
function(myexec workdir)
    execute_process(
        COMMAND ${ARGN}
        WORKING_DIRECTORY ${workdir}
        RESULT_VARIABLE result
        ERROR_VARIABLE error
        OUTPUT_VARIABLE output)
    if(NOT result STREQUAL 0)
        message(FATAL_ERROR
        "Error Executing Command : ${ARGN}\n"
        "WorkDir: ${workdir}\n"
        "Result: ${result} \n"
        "Error: ${error} \n"
        "Output: ${output}")
    endif()
endfunction()

function(download_winflexbison)
    set(WINFLEXBISON_URL  "https://github.com/lexxmark/winflexbison/releases/download/v2.5.22/win_flex_bison-2.5.22.zip")
    set(localdir ${CMAKE_CURRENT_BINARY_DIR}/winflexbison)
    set(localzip ${CMAKE_CURRENT_BINARY_DIR}/win_flex_bison.zip)
    file(MAKE_DIRECTORY ${localdir})
    if (NOT EXISTS ${localzip})
        file(DOWNLOAD ${WINFLEXBISON_URL} ${CMAKE_CURRENT_BINARY_DIR}/win_flex_bison.zip SHOW_PROGRESS)
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${localzip} WORKING_DIRECTORY ${localdir})
    if (NOT EXISTS ${localdir}/win_flex.exe)
        message(FATAL_ERROR "Cannot find win flex bison in extracted zip")
    endif()
    if (NOT EXISTS ${localdir}/win_bison.exe)
        message(FATAL_ERROR "Cannot find win flex bison in extracted zip")
    endif()
    SET(FLEX_EXECUTABLE     ${localdir}/win_flex.exe CACHE PATH "Flex executable" FORCE)
    SET(BISON_EXECUTABLE    ${localdir}/win_bison.exe CACHE PATH "Bison executable" FORCE)
    SET(WINFLEXBISON_DIR    ${localdir} CACHE PATH "win flex bison" FORCE)
endfunction()

function(build_bison)
    set(builddir        ${PROJECT_BINARY_DIR}/tools/bison)
    set(srcdir          ${builddir}/bison-3.5)
    file(MAKE_DIRECTORY ${builddir})

    if (NOT EXISTS ${srcdir})
        file(DOWNLOAD http://ftp.gnu.org/gnu/bison/bison-3.5.tar.gz ${builddir}/bison.tar.gz)
        myexec(${builddir} tar zxf ${builddir}/bison.tar.gz)
    endif()

    if (NOT EXISTS ${srcdir}/Makefile)
        myexec(${srcdir} ./configure --prefix=${builddir})
    endif()

    if (NOT EXISTS ${builddir}/bin/bison)
        myexec(${srcdir} make install)
    endif()

    find_program(BISON_EXECUTABLE bison HINTS ${builddir}/bin)
endfunction()


function(find_bison)
    if (EXISTS ${BISON_EXECUTABLE})
        return()
    endif()
    if (WIN32 and EXISTS ${WINFLEXBISON_DIR} and EXISTS ${WINFLEXBISON_DIR}/win_bison.exe)
        SET(BISON_EXECUTABLE  ${WINFLEXBISON_DIR}/win_bison.exe CACHE PATH  "Flex executable" FORCE)
        return()
    endif()
    find_package(Bison QUIET)
    if (EXISTS ${BISON_EXECUTABLE})
        return()
    endif()
    if (WIN32)
        download_winflexbison()
    else()
        message(FATAL_ERROR "Cannot Find Bison")
    endif()
endfunction()

function(find_flex)
    if (EXISTS ${FLEX_EXECUTABLE})
        return()
    endif()

    if (EXISTS ${WINFLEXBISON_DIR}/win_flex.exe)
        SET(FLEX_EXECUTABLE  ${WINFLEXBISON_DIR}/win_flex.exe CACHE PATH "Flex executable" FORCE)
        return()
    endif()

    find_package(FLEX QUIET)
    if (EXISTS ${FLEX_EXECUTABLE})
        return()
    endif()
    if (WIN32)
        download_winflexbison()
    else()
        message(FATAL_ERROR "Cannot Find Flex")
    endif()
endfunction()

set(LEXYACC_CPP ${CMAKE_CURRENT_LIST_DIR}/LexYacc.cpp)

function(find_or_create_lexyacc)
    if (EXISTS ${LEXYACC_BINARY})
        return()
    endif()
    set(outdir ${CMAKE_CURRENT_BINARY_DIR}/buildtool_lexyacc)
    file(MAKE_DIRECTORY ${outdir})
    file(WRITE ${outdir}/CMakeLists.txt "project(lexyacc)\nset(CMAKE_CXX_STANDARD 17)\nadd_executable(lexyacc ${LEXYACC_CPP})\ninstall(TARGETS lexyacc)")
    set(CMD ${CMAKE_COMMAND})
    list(APPEND CMD -DCMAKE_INSTALL_PREFIX=${outdir})
    list(APPEND CMD ".")
    myexec(${outdir} ${CMD})
    myexec(${outdir} ${CMAKE_COMMAND} --build . -j --target install)
    find_program(exe lexyacc HINTS ${outdir} ${outdir}/bin)
    if (NOT EXISTS ${exe})
        message(FATAL_ERROR "Cannot build lexyacc in ${outdir}")
    endif()
    set(LEXYACC_BINARY ${exe} CACHE PATH "Lex Yacc Exe" FORCE)
endfunction()

function(target_add_lexyacc target lyfile)
    find_flex()
    find_bison()
    find_or_create_lexyacc()

    if (NOT DEFINED FLEX_INCLUDE)
        get_filename_component(FLEX_INCLUDE ${FLEX_EXECUTABLE} DIRECTORY)
    endif()
    cmake_parse_arguments(lexyacc "" "NAME" "" ${ARGN})

    if (NOT lexyacc_NAME)
        get_filename_component(lexyacc_NAME ${lyfile} NAME_WE)
    endif()

    get_filename_component(srcdir ${lyfile} DIRECTORY)

    set(lytgt ${target}_lexyacc_${lexyacc_NAME})
    set(outdir ${PROJECT_BINARY_DIR}/${lytgt})
    file(MAKE_DIRECTORY ${outdir})
    set(yh ${outdir}/${lexyacc_NAME}.yacc.h)
    set(yc ${outdir}/${lexyacc_NAME}.yacc.cpp)
    set(lc ${outdir}/${lexyacc_NAME}.flex.cpp)
    set(yy ${outdir}/${lexyacc_NAME}.y)
    set(ll ${outdir}/${lexyacc_NAME}.l)
    set(hh ${outdir}/${lexyacc_NAME}.ly.h)

    set(outputs ${yy} ${yh} ${yc} ${ll} ${lc} ${hh})

    add_custom_command(
        OUTPUT  ${yy} ${yh} ${yc} ${ll} ${lc} ${hh}
        DEPENDS ${LEXYACC_BINARY} ${BISON_EXECUTABLE} ${FLEX_EXECUTABLE} ${lyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND ${LEXYACC_BINARY} ${lyfile} --outdir ${outdir} --prefix ${lexyacc_NAME}
        COMMAND ${BISON_EXECUTABLE} -o ${yc} --name-prefix=${lexyacc_NAME} --language=c++ --defines=${yh} ${yy}
        COMMAND ${FLEX_EXECUTABLE} -o${lc} --c++ --prefix=${lexyacc_NAME} ${ll}
        )
    target_sources(${target} PRIVATE ${lyfile} ${outputs})
    target_include_directories(${target} PRIVATE ${FLEX_INCLUDE} ${outdir})

    if (MSVC)
        set_source_files_properties(${lc} PROPERTIES COMPILE_FLAGS "-wd4005 -wd4065")
        set_source_files_properties(${yc} PROPERTIES COMPILE_FLAGS "-wd4065 -wd4127")
    endif()
    # TODO
    #add_library(${lytgt} ${outputs})
    #target_include_directories(${lytgt} PRIVATE ${FLEX_INCLUDE} ${srcdir})
    #target_include_directories(${lytgt} PUBLIC ${outdir})
    #target_link_libraries(${target} PRIVATE ${lytgt})
endfunction()
