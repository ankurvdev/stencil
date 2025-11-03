include_guard()
include(FetchContent)

FetchContent_Declare(
  tinyxml2
  GIT_REPOSITORY https://github.com/leethomason/tinyxml2
  GIT_TAG        master
  SOURCE_SUBDIR .
  GIT_PROGRESS FALSE GIT_SHALLOW 1 SYSTEM
  SYSTEM
  FIND_PACKAGE_ARGS NAMES tinyxml2
)

if (COMMAND vcpkg_install)
    vcpkg_install(tinyxml2)
endif()
set(tinyxml2_BUILD_TESTING OFF CACHE BOOL "Turn off tinyxml2 testing")
FetchContent_MakeAvailable(tinyxml2)
if (COMMAND SupressWarningForTarget)
    SupressWarningForTarget(tinyxml2)
endif()
