include_guard()
FetchContent_Declare(
    Boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG        boost-1.84.0
    SOURCE_SUBDIR .
    GIT_PROGRESS TRUE
    FIND_PACKAGE_ARGS
)

set(BOOST_INCLUDE_LIBRARIES beast url)
set(BOOST_ENABLE_CMAKE ON)

if (COMMAND vcpkg_install)
    vcpkg_install(boost-beast)
    vcpkg_install(boost-url)
endif()

FetchContent_MakeAvailable(Boost)
if (COMMAND SupressWarningForTarget)
    SupressWarningForTarget(boost_chrono)
    SupressWarningForTarget(boost_container)
    SupressWarningForTarget(boost_context)
    SupressWarningForTarget(boost_exception)
    SupressWarningForTarget(boost_date_time)
    SupressWarningForTarget(boost_coroutine)
    SupressWarningForTarget(boost_url)
endif()
if (NOT TARGET Boost::boost)
    message(WARNING "Cannot find boost beast")
endif()
if (MINGW AND TARGET boost_beast)
    target_link_libraries(boost_beast INTERFACE ws2_32 mswsock)
endif()
if (MINGW AND TARGET Boost::boost)
    target_link_libraries(Boost::boost INTERFACE ws2_32 mswsock)
endif()