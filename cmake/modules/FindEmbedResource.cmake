include_guard()

FetchContent_Declare(
  embedresource
  GIT_REPOSITORY https://github.com/ankurvdev/embedresource
  GIT_TAG        v0.0.5
  SOURCE_SUBDIR .
  FIND_PACKAGE_ARGS NAMES EmbedResource
)

if (COMMAND vcpkg_install)
    vcpkg_install(embedresource)
endif()

FetchContent_MakeAvailable(embedresource)
