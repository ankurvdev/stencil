include_guard()
include(FetchContent)

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts
  GIT_TAG        v3.2.1
  GIT_SHALLOW 1
  SYSTEM
  SOURCE_SUBDIR ./include
  FIND_PACKAGE_ARGS NAMES cxxopts
)

if (COMMAND vcpkg_install)
    vcpkg_install(cxxopts)
endif()

find_package(cxxopts CONFIG)
if(NOT cxxopts_FOUND)
  FetchContent_MakeAvailable(cxxopts)
  add_library(cxxopts INTERFACE)
  add_library(cxxopts::cxxopts ALIAS cxxopts)

  target_include_directories(cxxopts INTERFACE
      $<BUILD_INTERFACE:${cxxopts_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )
endif()
