include_guard()
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio
    GIT_TAG        asio-1-32-0
    SOURCE_SUBDIR .
    GIT_PROGRESS TRUE
    SYSTEM
    FIND_PACKAGE_ARGS NAMES asio
)

if (COMMAND vcpkg_install)
    vcpkg_install(asio)
endif()

FetchContent_MakeAvailable(asio)
