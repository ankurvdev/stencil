include_guard()
include(FetchContent)

FetchContent_Declare(
  rapidjson
  GIT_REPOSITORY https://github.com/Tencent/rapidjson
  GIT_TAG        v1.1.0
  GIT_PROGRESS TRUE
  GIT_SHALLOW 1
  SYSTEM
  SOURCE_SUBDIR ./include
  FIND_PACKAGE_ARGS NAMES RapidJSON
)

if (COMMAND vcpkg_install)
    vcpkg_install(rapidjson)
endif()

find_package(rapidjson CONFIG)
if(NOT rapidjson_FOUND)
  set(RAPIDJSON_BUILD_CXX17 ON)
  set(RAPIDJSON_BUILD_DOC OFF)
  set(RAPIDJSON_BUILD_EXAMPLES OFF)
  set(RAPIDJSON_BUILD_TESTS OFF)
  set(RAPIDJSON_ENABLE_INSTRUMENTATION_OPT OFF)

  FetchContent_MakeAvailable(rapidjson)

  add_library(rapidjson INTERFACE)
  target_include_directories(rapidjson SYSTEM INTERFACE "${rapidjson_SOURCE_DIR}/include")
endif()