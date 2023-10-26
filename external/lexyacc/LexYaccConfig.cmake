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
        if ((EXISTS ${WINFLEXBISON_DIR}) AND (EXISTS ${WINFLEXBISON_DIR}/win_bison.exe))
            set(BISON_EXECUTABLE ${WINFLEXBISON_DIR}/win_bison.exe CACHE PATH  "Bison executable" FORCE)
            return()
        endif()
    endif()
    find_package(BISON  QUIET)
    if ((EXISTS ${BISON_EXECUTABLE}) AND ("${BISON_VERSION}" GREATER_EQUAL "3.5"))
        message(STATUS "Found Bison ${BISON_EXECUTABLE}:${BISON_VERSION}")
        return()
    endif()

    set(BISON_EXECUTABLE BISON_EXECUTABLE-NOTFOUND CACHE PATH "Bison executable" FORCE)

    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        find_program(BISON_EXECUTABLE win_bison)
        if (EXISTS ${BISON_EXECUTABLE})
            return()
        endif()
    endif()

    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
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
        if ((EXISTS ${WINFLEXBISON_DIR}) AND (EXISTS ${WINFLEXBISON_DIR}/win_flex.exe))
            set(FLEX_EXECUTABLE ${WINFLEXBISON_DIR}/win_flex.exe CACHE PATH  "Flex executable" FORCE)
            return()
        endif()
    endif()

    find_package(FLEX QUIET)
    if (EXISTS ${FLEX_EXECUTABLE})
        return()
    endif()
    message(STATUS "Flex Include Dirs: ${FLEX_INCLUDE_DIR} Flex Exec: ${FLEX_EXECUTABLE}")

    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        find_program(FLEX_EXECUTABLE win_flex)
        if (EXISTS ${FLEX_EXECUTABLE})
            return()
        endif()
    endif()

    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        download_winflexbison()
    else()
        build_flex()
    endif()
endfunction()

set(LexYacc_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "LexYacc Dir")

macro(build_lexyacc)
    if (NOT EXISTS "${LEXYACC_EXECUTABLE}" AND NOT TARGET lexyacc)
    if (CMAKE_CROSSCOMPILING)
        unset(ENV{CMAKE_CXX_COMPILER})
        unset(ENV{CMAKE_C_COMPILER})
        unset(ENV{CC})
        unset(ENV{CXX})
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lexyacc-build")

        set(CMD "${CMAKE_COMMAND}" "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/lexyacc-install")
        if (CMAKE_GENERATOR)
            list(APPEND CMD "-G" "${CMAKE_GENERATOR}")
        endif()

        list(APPEND CMD "${LexYacc_DIR}")

        execute_process(COMMAND ${CMD} WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lexyacc-build")
        execute_process(COMMAND "${CMAKE_COMMAND}" --build  "${CMAKE_CURRENT_BINARY_DIR}/lexyacc-build")
        execute_process(COMMAND "${CMAKE_COMMAND}" --install "${CMAKE_CURRENT_BINARY_DIR}/lexyacc-build" --prefix "${CMAKE_CURRENT_BINARY_DIR}/lexyacc-install")
        find_program(LEXYACC_EXECUTABLE REQUIRED NAMES lexyacc PATHS "${CMAKE_CURRENT_BINARY_DIR}/lexyacc-install/bin")
    else()
        set(LEXYACC_SKIP_INSTALL CACHE BOOL ON "Do not install lexyacc")
        add_subdirectory("${LexYacc_DIR}" lexyacc)
    endif()
    endif()
endmacro()

function(_target_add_lexyacc target lyfile lexyacc_NAME)
    get_filename_component(lyfile "${lyfile}" ABSOLUTE)

    if (NOT EXISTS "${FLEX_INCLUDE_DIR}")
        get_filename_component(bindir "${FLEX_EXECUTABLE}" DIRECTORY)
        if (EXISTS "${bindir}/FlexLexer.h")
            set(FLEX_INCLUDE_DIR "${bindir}" CACHE PATH "Flex Include" FORCE)
        elseif(EXISTS "${LexYacc_DIR}/FlexLexer.h")
            set(FLEX_INCLUDE_DIR "${LexYacc_DIR}" CACHE PATH "Flex Include" FORCE)
        endif()
    endif()

    get_filename_component(srcdir "${lyfile}" DIRECTORY)

    set(lytgt ${target}_lexyacc_${lexyacc_NAME})
    set(outdir "${CMAKE_CURRENT_BINARY_DIR}/${lytgt}")

    file(MAKE_DIRECTORY ${outdir})
    set(yh "${outdir}/${lexyacc_NAME}.yacc.h")
    set(yc "${outdir}/${lexyacc_NAME}.yacc.cpp")
    set(lc "${outdir}/${lexyacc_NAME}.flex.cpp")
    set(yy "${outdir}/${lexyacc_NAME}.y")
    set(ll "${outdir}/${lexyacc_NAME}.l")
    set(hh "${outdir}/${lexyacc_NAME}.ly.h")

    set(outputs "${yy}" "${yh}" "${yc}" "${ll}" "${lc}" "${hh}")
    if (NOT EXISTS "${LEXYACC_EXECUTABLE}")
        find_program(LEXYACC_EXECUTABLE lexyacc)
    endif()
    if (NOT EXISTS "${LEXYACC_EXECUTABLE}")
        add_custom_command(
            OUTPUT  "${yy}" "${ll}"
            COMMAND lexyacc "${lyfile}" --outdir "${outdir}" --prefix ${lexyacc_NAME}
            DEPENDS lexyacc "${lyfile}"
        )
    else()
        add_custom_command(
            OUTPUT  "${yy}" "${ll}"
            COMMAND "${LEXYACC_EXECUTABLE}" "${lyfile}" --outdir "${outdir}" --prefix ${lexyacc_NAME}
            DEPENDS "${LEXYACC_EXECUTABLE}" "${lyfile}"
        )
    endif()

    add_custom_command(
        OUTPUT  "${yh}" "${yc}" "${lc}" "${hh}"
        COMMAND "${BISON_EXECUTABLE}" -o "${yc}" --name-prefix=${lexyacc_NAME} --language=c++ --defines="${yh}" "${yy}"
        COMMAND "${FLEX_EXECUTABLE}" -o"${lc}" --c++ --prefix=${lexyacc_NAME} "${ll}"
        DEPENDS "${BISON_EXECUTABLE}" "${FLEX_EXECUTABLE}" "${ll}" "${yy}"
    )

    target_sources(${target} PRIVATE "${lyfile}" ${outputs})
    target_include_directories(${target} PUBLIC "${outdir}")
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

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        target_compile_options(${target} PRIVATE /W3 /WX-)

        # 4244 return type conversion possible loss of data
        # 4365 signed unsigned mismatch
        # 4626 assignment operator implicitly defined as deleted
        # 4625 copy constructor implicitly defined as deleted
        # 5027 move assignment operator implicitly defined as deleted
        # 4668 is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
        set_source_files_properties("$lc}" PROPERTIES COMPILE_FLAGS "/W3")
        set_source_files_properties("$yc}" PROPERTIES COMPILE_FLAGS "/W3")
    elseif((${CMAKE_CXX_COMPILER_ID} STREQUAL GNU) OR (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang))
        set_source_files_properties(${lc} PROPERTIES COMPILE_FLAGS "-Wno-everything")
        set_source_files_properties(${yc} PROPERTIES COMPILE_FLAGS "-Wno-everything")
    endif()
endfunction()


macro(add_lexyacc_library)
    find_flex()
    find_bison()
    cmake_parse_arguments("" "" "TARGET" "LYFILE;SOURCES" ${ARGN})
    #message(FATAL_ERROR "${ARGN} :: ${_TARGET} ::${_LYFILE}")
    add_library(${_TARGET} OBJECT ${_SOURCES})
    _target_add_lexyacc(${_TARGET} "${_LYFILE}" ${_TARGET})
endmacro()