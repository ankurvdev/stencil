include_guard()
include(FetchContent)

FetchContent_Declare(
  date
  GIT_REPOSITORY https://github.com/HowardHinnant/date
  GIT_TAG        v3.0.4
  SOURCE_SUBDIR .
  GIT_PROGRESS FALSE GIT_SHALLOW 1 SYSTEM
  FIND_PACKAGE_ARGS NAMES date
)

if (COMMAND vcpkg_install)
    vcpkg_install(date)
endif()

FetchContent_MakeAvailable(date)