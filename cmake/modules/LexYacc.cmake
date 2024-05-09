include_guard()
include(FetchContent)

FetchContent_Declare(
  lexyacc
  GIT_REPOSITORY https://github.com/ankurvdev/lexyacc
  GIT_TAG        main
  GIT_SHALLOW 1
  SOURCE_SUBDIR .
  FIND_PACKAGE_ARGS NAMES LexYacc
)

if (COMMAND vcpkg_install)
    vcpkg_install(ankurvdev-lexyacc)
endif()

FetchContent_MakeAvailable(lexyacc)
