include_guard()
include(FetchContent)

FetchContent_Declare(
  embedresource
  GIT_REPOSITORY https://github.com/ankurvdev/embedresource
  GIT_TAG        clangfixes
  GIT_PROGRESS FALSE GIT_SHALLOW ON SYSTEM
  SOURCE_SUBDIR .
  FIND_PACKAGE_ARGS NAMES EmbedResource
)

if (COMMAND vcpkg_install)
    #vcpkg_install(ankurvdev-embedresource)
endif()

FetchContent_MakeAvailable(embedresource)
