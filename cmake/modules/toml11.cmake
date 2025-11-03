include_guard()
include(FetchContent)

FetchContent_Declare(
  toml11
  GIT_REPOSITORY https://github.com/ToruNiina/toml11
  GIT_TAG        v4.4.0
  SOURCE_SUBDIR .
  GIT_PROGRESS FALSE GIT_SHALLOW 1 SYSTEM
  SYSTEM
  FIND_PACKAGE_ARGS NAMES toml11
)

if (COMMAND vcpkg_install)
    vcpkg_install(toml11)
endif()

FetchContent_MakeAvailable(toml11)
if (COMMAND SupressWarningForTarget)
    # SupressWarningForTarget(toml11)
endif()
