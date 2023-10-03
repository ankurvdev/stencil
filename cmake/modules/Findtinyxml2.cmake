include_guard()

FetchContent_Declare(
  tinyxml2
  GIT_REPOSITORY https://github.com/leethomason/tinyxml2
  GIT_TAG        master
  SOURCE_SUBDIR .
  FIND_PACKAGE_ARGS NAMES tinyxml2
)

if (COMMAND vcpkg_install)
    vcpkg_install(tinyxml2)
endif()

FetchContent_MakeAvailable(tinyxml2)
if (COMMAND SupressWarningForTarget AND TARGET tinyxml2)
    SupressWarningForTarget(tinyxml2)
endif()
