include_guard()
include(FetchContent)

FetchContent_Declare(
  httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
  GIT_TAG        v0.16.0
  SOURCE_SUBDIR .
  GIT_PROGRESS TRUE
  GIT_SHALLOW 1
  SYSTEM
  SOURCE_SUBDIR ./include
  FIND_PACKAGE_ARGS NAMES httplib
)

if (COMMAND vcpkg_install)
    vcpkg_install(cpp-httplib)
endif()
find_package(httplib QUIET)
if (NOT httplib_FOUND)
  find_path(CPP_HTTPLIB_INCLUDE_DIRS "httplib.h")
  if (NOT EXISTS "${CPP_HTTPLIB_INCLUDE_DIRS}")
    FetchContent_MakeAvailable(httplib)
    set(CPP_HTTPLIB_INCLUDE_DIRS "${httplib_SOURCE_DIR}")
  endif()
  if (NOT EXISTS "${CPP_HTTPLIB_INCLUDE_DIRS}")
    message(FATAL_ERROR "Cannot find httplib.h at ${CPP_HTTPLIB_INCLUDE_DIRS}")
  endif()

  if (NOT TARGET httplib)
      add_library(httplib INTERFACE)
      target_include_directories(httplib SYSTEM INTERFACE "${CPP_HTTPLIB_INCLUDE_DIRS}")
      target_compile_definitions(httplib INTERFACE HAVE_CPP_HTTPLIB=1)
      if (WIN32 OR MSYS)
          target_link_libraries(httplib INTERFACE ws2_32)
          target_compile_definitions(httplib INTERFACE _WIN32_WINNT=_WIN32_WINNT_WIN10 WINVER=_WIN32_WINNT_WIN10)
      endif()
      add_library(httplib::httplib ALIAS httplib)
  endif()
endif()