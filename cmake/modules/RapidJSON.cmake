include_guard()
include(FetchContent)

FetchContent_Declare(
  rapidjson
  GIT_REPOSITORY https://github.com/Tencent/rapidjson
  GIT_TAG        f9d53419e912910fd8fa57d5705fa41425428c35
  GIT_PROGRESS TRUE
  SYSTEM
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

  FetchContent_Populate(rapidjson)
  add_library(rapidjson INTERFACE)
  target_include_directories(rapidjson SYSTEM INTERFACE "${rapidjson_SOURCE_DIR}/include")
endif()