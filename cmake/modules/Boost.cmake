include_guard()
set(BOOST_VERSION 1.90.0)

FetchContent_Declare(
    boostbuild
    OVERRIDE_FIND_PACKAGE TRUE
    URL https://github.com/boostorg/boost/releases/download/boost-${BOOST_VERSION}/boost-${BOOST_VERSION}-cmake.tar.gz
    SYSTEM
)

if (NOT DEFINED BOOST_INCLUDE_LIBRARIES)
    message(FATAL_ERROR "BOOST_INCLUDE_LIBRARIES not defined. Please define libraries to include")
endif()

set(BOOST_ENABLE_CMAKE ON)

if (COMMAND vcpkg_install)
    foreach(item IN LISTS BOOST_INCLUDE_LIBRARIES)
        string(TOLOWER ${item} item_lower)
        string(REPLACE "_" "-" item_lower ${item_lower})
        set(vcpkg_boost_lib_name "boost-${item_lower}")
        list(APPEND vcpkg_boost_libs ${vcpkg_boost_lib_name})
    endforeach()
    vcpkg_install(${vcpkg_boost_libs})
endif()

find_package(Boost QUIET OPTIONAL_COMPONENTS ${BOOST_INCLUDE_LIBRARIES})

if(NOT Boost_FOUND)
    FetchContent_GetProperties(boostbuild)
    if(NOT boostbuild_POPULATED)
        FetchContent_MakeAvailable(boostbuild)
    endif()
endif()

if (COMMAND SupressWarningForTarget)
    foreach(item IN LISTS BOOST_INCLUDE_LIBRARIES)
        set(target_name "boost_${item}")
        if (TARGET ${target_name})
            SupressWarningForTarget(${target_name})
        endif()
        if ((TARGET boost_${item}) AND (NOT TARGET Boost::${item}))
            add_library(Boost::${item} ALIAS boost_${item})
        endif()
    endforeach()
    SupressWarningForTarget(boost_atomic)
    SupressWarningForTarget(boost_container)
    SupressWarningForTarget(boost_context)
    SupressWarningForTarget(boost_date_time)
    SupressWarningForTarget(boost_filesystem)
endif()

foreach(item IN LISTS BOOST_INCLUDE_LIBRARIES)
    if (TARGET boost_${item} AND (NOT TARGET Boost::${item}))
        add_library(Boost::${item} ALIAS boost_${item})
    endif()
    if (NOT TARGET Boost::${item})
        message(FATAL_ERROR "Cannot find target Boost::${item}")
    endif()
endforeach()
