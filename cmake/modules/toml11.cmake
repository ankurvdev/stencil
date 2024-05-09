include_guard()
include(FetchContent)

FetchContent_Declare(
  toml11
  GIT_REPOSITORY https://github.com/ToruNiina/toml11
  GIT_TAG        master
  SOURCE_SUBDIR .
  GIT_SHALLOW 1
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
