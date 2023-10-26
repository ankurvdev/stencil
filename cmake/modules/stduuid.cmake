include_guard()
include(FetchContent)

FetchContent_Declare(
  stduuid
  GIT_REPOSITORY https://github.com/mariusbancila/stduuid
  GIT_TAG        v1.2.3
  SOURCE_SUBDIR .
  GIT_PROGRESS TRUE
  FIND_PACKAGE_ARGS NAMES stduuid
)

if (COMMAND vcpkg_install)
    vcpkg_install(stduuid)
endif()

FetchContent_MakeAvailable(stduuid)
