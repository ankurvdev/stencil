include_guard()
FetchContent_Declare(
    boostbeast
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG        boost-1.83.0
    SOURCE_SUBDIR .
    GIT_PROGRESS TRUE
    FIND_PACKAGE_ARGS NAMES Boost COMPONENTS beast
)

set(BOOST_INCLUDE_LIBRARIES beast)
set(BOOST_ENABLE_CMAKE ON)

if (COMMAND vcpkg_install)
    vcpkg_install(boost-beast)
endif()

FetchContent_MakeAvailable(boostbeast)
if (COMMAND SupressWarningForTarget)
    SupressWarningForTarget(boost_chrono)
    SupressWarningForTarget(boost_container)
    SupressWarningForTarget(boost_context)
    SupressWarningForTarget(boost_exception)
    SupressWarningForTarget(boost_date_time)
    SupressWarningForTarget(boost_coroutine)
endif()
if (NOT TARGET Boost::beast)
    message(FATAL_ERROR "Cannot find boost beast")
endif()