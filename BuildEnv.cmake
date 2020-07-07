set(CMAKE_CXX_STANDARD 20)

function(_FixFlags name)
    cmake_parse_arguments("" "" "" "VALUE;EXCLUDE;APPEND" ${ARGN})
    if (NOT _VALUE)
        set(_VALUE ${${name}})
    endif()
    set(origlist ${_VALUE})

    string(REPLACE " " ";" origlist ${origlist})

    foreach (val ${origlist})
        list(APPEND cflags ${val})
    endforeach()
    foreach(excl ${_EXCLUDE})
        list(FILTER cflags EXCLUDE REGEX ${excl})
    endforeach()

    list(APPEND cflags ${_APPEND})
    list(REMOVE_DUPLICATES cflags)
    list(JOIN cflags " " cflagsstr)

    message(STATUS "${name}: ${_VALUE} ==> ${cflagsstr}")
    set(${name} "${cflagsstr}" CACHE STRING "" FORCE)
endfunction()

function(EnableStrictCompilation)
    file(TIMESTAMP ${CMAKE_CURRENT_LIST_FILE} filetime)

    if (STRICT_COMPILATION_MODE AND STRICT_COMPILATION_MODE STREQUAL ${filetime})
        return()
    endif()

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        set(extraflags 
            -Wall       # Enable all errors
#            -WX     # All warnings as errors
            /permissive- # strict compilation
            #suppression list
            /wd4068  # unknown pragma
            /wd4514  # unreferenced inline function has been removed
            /wd4820  # bytes padding added after data member in struct
            /wd5045  # Spectre mitigation insertion
            /wd4710  # Function not inlined . TODO remove this once msvc crt does not throw this error
            /showIncludes
        )

        set(exclusions "[-/]W[a-zA-Z1-9]+" "[-/]permissive?")

        _FixFlags(CMAKE_C_FLAGS EXCLUDE ${exclusions} APPEND ${extraflags})
        _FixFlags(CMAKE_CXX_FLAGS EXCLUDE ${exclusions}  APPEND ${extraflags})
        _FixFlags(CMAKE_C_FLAGS_DEBUG APPEND /RTCcsu)

    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL ClangTODO)
        set(extraflags 
            /Wall   # Enable all errors
            /WX     # All warnings as errors
            /permissive- # strict compilation
            #suppression list
            -Wno-unknown-argument
            -Wno-unknown-pragmas
            -Wno-unknown-warning-option
            -Wno-documentation
            -Wno-covered-switch-default
            -Wno-documentation-unknown-command
        )

        set(exclusions "[-/]W[a-zA-Z1-9]+" "[-/]permissive?")

        _FixFlags(CMAKE_C_FLAGS EXCLUDE ${exclusions} APPEND ${extraflags})
        _FixFlags(CMAKE_CXX_FLAGS EXCLUDE ${exclusions}  APPEND ${extraflags})

        message(FATAL_ERROR "Unknown compiler : ${CMAKE_CXX_COMPILER_ID}")
    else()
        message(STATUS "CMAKE_C_FLAGS                  : ${CMAKE_C_FLAGS}")
        message(STATUS "CMAKE_C_FLAGS_DEBUG            : ${CMAKE_C_FLAGS_DEBUG}")
        message(STATUS "CMAKE_C_FLAGS_MINSIZEREL       : ${CMAKE_C_FLAGS_MINSIZEREL}")
        message(STATUS "CMAKE_C_FLAGS_RELEASE          : ${CMAKE_C_FLAGS_RELEASE}")
        message(STATUS "CMAKE_C_FLAGS_RELWITHDEBINFO   : ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
        message(STATUS "CMAKE_CXX_FLAGS                : ${CMAKE_CXX_FLAGS}")
        message(STATUS "CMAKE_CXX_FLAGS_DEBUG          : ${CMAKE_CXX_FLAGS_DEBUG}")
        message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL     : ${CMAKE_CXX_FLAGS_MINSIZEREL}")
        message(STATUS "CMAKE_CXX_FLAGS_RELEASE        : ${CMAKE_CXX_FLAGS_RELEASE}")
        message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO : ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
        message(STATUS "CMAKE_SYSTEM_PROCESSOR         : ${CMAKE_SYSTEM_PROCESSOR}")

        message(FATAL_ERROR "Unknown compiler : ${CMAKE_CXX_COMPILER_ID}")
    endif()
    set(STRICT_COMPILATION_MODE ${filetime} CACHE INTERNAL "Is Strict Compilation mode enabled" FORCE)
endfunction()

EnableStrictCompilation()
