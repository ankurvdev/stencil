include_guard()
set(BOOST_VERSION 1.85.0)
FetchContent_Declare(
    boostbuild
    URL https://github.com/boostorg/boost/releases/download/boost-${BOOST_VERSION}/boost-${BOOST_VERSION}-cmake.tar.gz
    SYSTEM
)

set(BOOST_INCLUDE_LIBRARIES beast asio url)
set(BOOST_ENABLE_CMAKE ON)

if (COMMAND vcpkg_install)
    vcpkg_install(boost-beast)
    vcpkg_install(boost-url)
endif()

find_package(Boost QUIET COMPONENTS beast url)
if(NOT Boost_FOUND)
    FetchContent_GetProperties(boostbuild)
    if(NOT boostbuild_POPULATED)
        FetchContent_Populate(boostbuild)
        add_subdirectory(${boostbuild_SOURCE_DIR} ${boostbuild_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endif()

if (COMMAND SupressWarningForTarget)
    SupressWarningForTarget(boost_chrono)
    SupressWarningForTarget(boost_container)
    SupressWarningForTarget(boost_context)
    SupressWarningForTarget(boost_exception)
    SupressWarningForTarget(boost_date_time)
    SupressWarningForTarget(boost_coroutine)
    SupressWarningForTarget(boost_url)
endif()