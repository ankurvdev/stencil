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
    set(FLEX_EXECUTABLE     ${localdir}/win_flex.exe    CACHE PATH "Flex executable"    FORCE)
    set(BISON_EXECUTABLE    ${localdir}/win_bison.exe   CACHE PATH "Bison executable"   FORCE)
    set(WINFLEXBISON_DIR    ${localdir}                 CACHE PATH "win flex bison"     FORCE)
endfunction()

function(build_flex)
    set(FLEXSRC https://github.com/westes/flex/archive/master.zip)

    set(builddir        ${PROJECT_BINARY_DIR}/tools/flex)
    set(srcdir          ${builddir}/src)
    set(localzip        ${builddir}/flex.zip)

    file(MAKE_DIRECTORY ${srcdir})

    file(DOWNLOAD ${FLEXSRC} ${localzip})
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${localzip} WORKING_DIRECTORY ${srcdir})
    execute_process(COMMAND ./autogen.sh WORKING_DIRECTORY ${srcdir}/flex-master)
    execute_process(COMMAND ./configure --prefix=${builddir} WORKING_DIRECTORY ${srcdir}/flex-master)
    execute_process(COMMAND make install WORKING_DIRECTORY ${srcdir}/flex-master)
    
    find_program(FLEX_EXECUTABLE flex HINTS ${builddir})
    if (NOT EXISTS ${FLEX_EXECUTABLE})
        message(FATAL_ERROR "Cannot build flex in ${builddir}")
    endif()
    set(FLEX_INCLUDE_DIR ${srcdir}/flex-cmake/src CACHE PATH "Flex Include Path" FORCE)
endfunction()

function(build_bison)
    set(builddir        ${PROJECT_BINARY_DIR}/tools/bison)
    set(srcdir          ${builddir}/src)
    set(localzip        ${builddir}/bison.tar.gz)
    file(MAKE_DIRECTORY ${srcdir})

    file(DOWNLOAD http://ftp.gnu.org/gnu/bison/bison-3.5.tar.gz ${localzip})
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${localzip} WORKING_DIRECTORY ${srcdir})
    execute_process(COMMAND ./configure --prefix=${builddir} WORKING_DIRECTORY ${srcdir}/bison-3.5)
    execute_process(COMMAND make install WORKING_DIRECTORY ${srcdir}/bison-3.5)
    find_program(BISON_EXECUTABLE bison HINTS ${builddir}/bin)
endfunction()

function(find_bison)
    if (EXISTS ${BISON_EXECUTABLE})
        return()
    endif()
      if (DEFINED WINFLEXBISON_DIR) 
        if ((EXISTS ${WINFLEXBISON_DIR}) and (EXISTS ${WINFLEXBISON_DIR}/win_bison.exe))
            set(BISON_EXECUTABLE ${WINFLEXBISON_DIR}/win_bison.exe CACHE PATH  "Bison executable" FORCE)
            return()
        endif()
    endif()
    find_package(BISON 3.5 QUIET)
    if (EXISTS ${BISON_EXECUTABLE})
        return()
    endif()
    if (WIN32)
        find_program(BISON_EXECUTABLE win_bison)
        if (EXISTS ${BISON_EXECUTABLE})
            return()
        endif()
    endif()
    
    if (WIN32)
        download_winflexbison()
    else()
        build_bison()
    endif()
endfunction()


function(find_flex)
    if (EXISTS ${FLEX_EXECUTABLE})
        return()
    endif()
    if (DEFINED WINFLEXBISON_DIR) 
        if ((EXISTS ${WINFLEXBISON_DIR}) and (EXISTS ${WINFLEXBISON_DIR}/win_flex.exe))
            set(FLEX_EXECUTABLE ${WINFLEXBISON_DIR}/win_flex.exe CACHE PATH  "Flex executable" FORCE)
            return()
        endif()
    endif()

    find_package(FLEX QUIET)
    if (EXISTS ${FLEX_EXECUTABLE})
        return()
    endif()
    message(STATUS "Flex Include Dirs: ${FLEX_INCLUDE_DIR} Flex Exec: ${FLEX_EXECUTABLE}")

    if (WIN32)
        find_program(FLEX_EXECUTABLE win_flex)
        if (EXISTS ${FLEX_EXECUTABLE})
            return()
        endif()
    endif()
    
    if (WIN32)
        download_winflexbison()
    else()
        build_flex()
    endif()
endfunction()

set(LexYacc_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "LexYacc Dir")

function(build_lexyacc)
    if (TARGET lexyacc)
        return()
    endif()
    add_executable(lexyacc ${LexYacc_DIR}/LexYacc.cpp)
endfunction()

function(find_or_create_lexyacc)
    if (TARGET lexyacc)
        return()
    endif()
    if (EXISTS ${LEXYACC_EXECUTABLE})
        return()
    endif()
    find_program(LEXYACC_EXECUTABLE lexyacc)
    if (EXISTS ${LEXYACC_EXECUTABLE})
        return()
    endif()
endfunction()

function(target_add_lexyacc target lyfile)
    find_bison()
    find_flex()
    find_or_create_lexyacc()
    get_filename_component(lyfile ${lyfile} ABSOLUTE)

    if (NOT EXISTS ${FLEX_INCLUDE_DIR})
        get_filename_component(bindir ${FLEX_EXECUTABLE} DIRECTORY)
        if (EXISTS ${bindir}/FlexLexer.h)
            set(FLEX_INCLUDE_DIR ${bindir} CACHE PATH "Flex Include" FORCE)
        else()
            set(FLEX_INCLUDE_DIR ${LexYacc_DIR} CACHE PATH "Flex Include" FORCE)
        endif()
    endif()
    
    cmake_parse_arguments(lexyacc "" "NAME" "" ${ARGN})

    if (NOT lexyacc_NAME)
        get_filename_component(lexyacc_NAME ${lyfile} NAME_WE)
    endif()

    get_filename_component(srcdir ${lyfile} DIRECTORY)

    set(lytgt ${target}_lexyacc_${lexyacc_NAME})
    set(outdir ${CMAKE_CURRENT_BINARY_DIR}/${lytgt})
    
    file(MAKE_DIRECTORY ${outdir})
    set(yh ${outdir}/${lexyacc_NAME}.yacc.h)
    set(yc ${outdir}/${lexyacc_NAME}.yacc.cpp)
    set(lc ${outdir}/${lexyacc_NAME}.flex.cpp)
    set(yy ${outdir}/${lexyacc_NAME}.y)
    set(ll ${outdir}/${lexyacc_NAME}.l)
    set(hh ${outdir}/${lexyacc_NAME}.ly.h)

    set(outputs ${yy} ${yh} ${yc} ${ll} ${lc} ${hh})
    if (TARGET lexyacc)  
        add_custom_command(
            OUTPUT  ${yy} ${ll} 
            COMMAND lexyacc ${lyfile} --outdir ${outdir} --prefix ${lexyacc_NAME}
            DEPENDS lexyacc ${lyfile}
        )
    else()
        add_custom_command(
            OUTPUT  ${yy} ${ll} 
            COMMAND ${LEXYACC_EXECUTABLE} ${lyfile} --outdir ${outdir} --prefix ${lexyacc_NAME}
            DEPENDS ${LEXYACC_EXECUTABLE} ${lyfile}
        )
    endif()

    add_custom_command(
        OUTPUT  ${yh} ${yc} ${lc} ${hh}
        COMMAND ${BISON_EXECUTABLE} -o ${yc} --name-prefix=${lexyacc_NAME} --language=c++ --defines=${yh} ${yy}
        COMMAND ${FLEX_EXECUTABLE} -o${lc} --c++ --prefix=${lexyacc_NAME} ${ll}
        DEPENDS ${BISON_EXECUTABLE} ${FLEX_EXECUTABLE} ${ll} ${yy}
    )

    target_sources(${target} PRIVATE ${lyfile} ${outputs})
    target_include_directories(${target} PRIVATE ${outdir})
    if (DEFINED FLEX_INCLUDE_DIRS)
        set(FLEX_INCLUDE_DIR ${FLEX_INCLUDE_DIRS} CACHE PATH "Flex Include dir" FORCE)
    endif()

    if (NOT DEFINED FLEX_INCLUDE_DIR)
        message(WARNING "Flex include dir not available ${FLEX_INCLUDE_DIR} :: ${FLEX_INCLUDE_DIRS}")
        target_include_directories(${target} PRIVATE ${LexYacc_DIR})
    else()
        message(STATUS "Flex include dir :: ${FLEX_INCLUDE_DIR} :: ${FLEX_INCLUDE_DIRS}")
        target_include_directories(${target} PRIVATE ${FLEX_INCLUDE_DIR})
    endif() 

    if (MSVC)
        set_source_files_properties(${lc} PROPERTIES COMPILE_FLAGS "-wd4005 -wd4065")
        set_source_files_properties(${yc} PROPERTIES COMPILE_FLAGS "-wd4065 -wd4127")
    endif()
endfunction()
