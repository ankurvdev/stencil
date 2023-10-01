include_guard()

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts
  GIT_TAG        master
  SOURCE_SUBDIR .
  FIND_PACKAGE_ARGS NAMES cxxopts
)

if (COMMAND vcpkg_install)
    vcpkg_install(cxxopts)
endif()

FetchContent_MakeAvailable(cxxopts)