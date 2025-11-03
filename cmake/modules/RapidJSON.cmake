include_guard()
include(FetchContent)

FetchContent_Declare(
  rapidjson
  GIT_REPOSITORY https://github.com/Tencent/rapidjson
  GIT_TAG        24b5e7a8b27f42fa16b96fc70aade9106cf7102f # accessed on 2025-02-26
  GIT_PROGRESS FALSE GIT_SHALLOW 1 SYSTEM
  SOURCE_SUBDIR ./include
  FIND_PACKAGE_ARGS NAMES RapidJSON
)

if (COMMAND vcpkg_install)
    vcpkg_install(rapidjson)
endif()

find_package(RapidJSON CONFIG)
if(NOT RapidJSON_FOUND)
  set(RAPIDJSON_BUILD_CXX17 ON)
  set(RAPIDJSON_BUILD_DOC OFF)
  set(RAPIDJSON_BUILD_EXAMPLES OFF)
  set(RAPIDJSON_BUILD_TESTS OFF)
  set(RAPIDJSON_ENABLE_INSTRUMENTATION_OPT OFF)

  FetchContent_MakeAvailable(rapidjson)

  add_library(rapidjson INTERFACE)
  target_include_directories(rapidjson SYSTEM INTERFACE "${rapidjson_SOURCE_DIR}/include")
endif()