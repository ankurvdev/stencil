include_guard()
include(FetchContent)

FetchContent_Declare(
  cpp-httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
  GIT_TAG        v0.13.1
  SOURCE_SUBDIR .
  GIT_PROGRESS TRUE
  FIND_PACKAGE_ARGS NAMES httplib
)

if (COMMAND vcpkg_install)
    vcpkg_install(cpp-httplib)
endif()

FetchContent_MakeAvailable(cpp-httplib)
