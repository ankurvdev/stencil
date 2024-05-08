include_guard()
FetchContent_Declare(
    BoostBuild
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG        boost-1.85.0
    GIT_PROGRESS TRUE
    GIT_SHALLOW 1
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
    FetchContent_GetProperties(BoostBuild)
    if(NOT BoostBuild_POPULATED)
        FetchContent_Populate(BoostBuild)
        add_subdirectory(${BoostBuild_SOURCE_DIR} ${BoostBuild_BINARY_DIR} EXCLUDE_FROM_ALL)
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

if (MINGW AND TARGET boost_beast)
    target_link_libraries(boost_beast INTERFACE ws2_32 mswsock)
endif()
if (MINGW AND TARGET Boost::beast)
    target_link_libraries(Boost::beast INTERFACE ws2_32 mswsock)
endif()