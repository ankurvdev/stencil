include_guard()
include(FetchContent)

FetchContent_Declare(
  embedresource
  GIT_REPOSITORY https://github.com/ankurvdev/embedresource
  GIT_TAG        v0.0.11
  SOURCE_SUBDIR .
  FIND_PACKAGE_ARGS NAMES embedresource
)

if (COMMAND vcpkg_install)
    vcpkg_install(ankurvdev-embedresource)
endif()

FetchContent_MakeAvailable(embedresource)
