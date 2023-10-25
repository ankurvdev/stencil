include_guard()

FetchContent_Declare(
  tomlplusplus
  GIT_REPOSITORY https://github.com/marzer/tomlplusplus
  GIT_TAG        master
  SOURCE_SUBDIR .
  FIND_PACKAGE_ARGS NAMES tomlplusplus
)

if (COMMAND vcpkg_install)
    vcpkg_install(tomlplusplus)
endif()

FetchContent_MakeAvailable(tomlplusplus)
