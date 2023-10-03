cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
set(INIT_SUBMODULE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/external)

include(BuildEnv)
include(FetchContent)

project(stencil VERSION 0.0.3)

include(CMakePackageConfigHelpers)

option(BUILD_TESTING "Build Test" ON)
option(STENCIL_INSTALL_BUILDTOOLS "Install Extra Build Tools with install" ON)

set(CMAKE_CXX_STANDARD 20)
EnableStrictCompilation()

FetchContent_Declare(
  tomlplusplus
  GIT_REPOSITORY https://github.com/marzer/tomlplusplus
  GIT_TAG        master
  SOURCE_SUBDIR .
  FIND_PACKAGE_ARGS NAMES tomlplusplus
)


FetchContent_Declare(
  tinyxml2
  GIT_REPOSITORY https://github.com/leethomason/tinyxml2
  GIT_TAG        master
  SOURCE_SUBDIR .
  FIND_PACKAGE_ARGS NAMES tinyxml2
)

FetchContent_Declare(
  boostbeast
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG        boost-1.83.0
  SOURCE_SUBDIR .
  GIT_PROGRESS TRUE
  FIND_PACKAGE_ARGS NAMES Boost COMPONENTS beast
)

FetchContent_Declare(
  stduuid
  GIT_REPOSITORY https://github.com/mariusbancila/stduuid
  GIT_TAG        v1.2.3
  SOURCE_SUBDIR .
  GIT_PROGRESS TRUE
  FIND_PACKAGE_ARGS NAMES stduuid
)

set(BOOST_INCLUDE_LIBRARIES beast)
set(BOOST_ENABLE_CMAKE ON)

find_package (Threads REQUIRED)

set(LexYacc_DIR       ${CMAKE_CURRENT_LIST_DIR}/external/lexyacc)

file(GLOB public_headers ${CMAKE_CURRENT_LIST_DIR}/include/stencil/*)

find_package(LexYacc)
find_package(cxxopts REQUIRED MODULE)
find_package(fmt REQUIRED MODULE)
find_package(EmbedResource REQUIRED MODULE)
find_package(date REQUIRED MODULE)

FetchContent_MakeAvailable(tomlplusplus)
FetchContent_MakeAvailable(embedresource)
FetchContent_MakeAvailable(tinyxml2)
FetchContent_MakeAvailable(stduuid)
FetchContent_MakeAvailable(boostbeast)
find_package(tsl-ordered-map)
find_package(RapidJSON)

if (NOT TARGET stduuid)
    add_library(stduuid INTERFACE)
    add_library(stduuid::stduuid ALIAS stduuid)
    target_include_directories(stduuid INTERFACE external/stduuid/include)
    set(UUIDHEADER "external/stduuid/include/uuid.h")
    list(APPEND public_headers "${UUIDHEADER}")
endif()

find_package(CommonMacros QUIET MODULE REQUIRED)
list(APPEND public_headers "${COMMONMACROS_INCLUDE_FILE}")

if (BUILD_TESTING)
    enable_testing()
    find_package(Catch2 REQUIRED MODULE)
    add_library(websvc INTERFACE)
    target_link_libraries(websvc INTERFACE Boost::beast)
endif()

build_lexyacc()

file(GLOB resfiles src/templates/*)

add_library(libstencil OBJECT
    src/ModelGenerator.cpp
    src/Binding.h
    src/DebugInfo.h
    src/GeneratedCodeFragment.h
    src/Generator.h
    src/IDL2.h
    src/IDL3Generics.h
    src/TemplateFragment.h
    src/Thrift.h
    src/tree.h
)
target_include_directories(libstencil PUBLIC src)
add_lexyacc_library(TARGET Thrift LYFILE src/Thrift.ly SOURCES src/Thrift.cpp)
target_include_directories(Thrift PUBLIC src)
target_link_libraries(Thrift PUBLIC CommonMacros fmt::fmt-header-only)
if (MSVC)
    # TOML11 has a nasty usage of insecure apis
    set_source_files_properties("src/ModelGenerator.cpp" PROPERTIES COMPILE_FLAGS "/D_CRT_SECURE_NO_WARNINGS")
endif()
target_link_libraries(libstencil PUBLIC Thrift)
target_add_resource(libstencil templates ${resfiles})
list(APPEND resfiles ${public_headers})
if (NOT BUILD_TESTING)
    target_add_resource(libstencil commonheaders ${resfiles})
endif()

target_link_libraries(libstencil PUBLIC
    tomlplusplus::tomlplusplus
    tinyxml2::tinyxml2
    tsl::ordered_map
    fmt::fmt-header-only
    CommonMacros
)

set(stencil_INCLUDE_PATH "${CMAKE_CURRENT_LIST_DIR}/include" CACHE PATH "Stencil include path")
include(stencilTargets)

target_link_libraries(stencil_runtime INTERFACE CommonMacros)

add_executable(stencil src/main.cpp)

set_target_properties(stencil PROPERTIES PUBLIC_HEADER "${public_headers}")

target_link_libraries(stencil PRIVATE libstencil Thrift cxxopts::cxxopts)

if (BUILD_TESTING)
    add_subdirectory(tests/CodegenRuntime)
endif()

if (BUILD_TESTING)
    enable_testing()
    find_package(TestCommon REQUIRED MODULE)
    file(GLOB resfiles CONFIGURE_DEPENDS tests/testdata/*)
    file(GLOB pidlfiles tests/*.pidl)

    add_executable(codegen_tests
        tests/Codegen/Test_ThriftGenerator.cpp
    )
    add_test(NAME codegen_tests COMMAND codegen_tests)
    target_include_directories(codegen_tests PRIVATE src include tests)
    target_link_libraries(codegen_tests PRIVATE libstencil Thrift TestCommon)
    target_add_resource(codegen_tests testdata ${resfiles} ${pidlfiles})
    target_compile_definitions(codegen_tests PRIVATE HAVE_EMBEDRESOURCE=1)
    add_executable(runtime_tests
        tests/HandwrittenRuntime/Test_Handwritten_CLOpts.cpp
        tests/HandwrittenRuntime/Test_Handwritten_Json.cpp
        tests/HandwrittenRuntime/Test_Handwritten_Binary.cpp
    )

    target_compile_definitions(runtime_tests PRIVATE HAVE_NLOHMANN_JSON)
    target_link_libraries(runtime_tests PRIVATE stencil_runtime)
    target_link_libraries(runtime_tests PRIVATE TestCommon websvc stencil::runtime)
    target_link_libraries (runtime_tests PRIVATE Threads::Threads)
    add_test(NAME runtime_tests COMMAND runtime_tests)

    target_add_resource(runtime_tests testdata ${resfiles})
    target_compile_definitions(runtime_tests PRIVATE HAVE_EMBEDRESOURCE=1)
    target_include_directories(runtime_tests PRIVATE src include tests)
 
endif()

configure_package_config_file(cmake/stencilConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/stencilConfig.cmake
    INSTALL_DESTINATION share/stencil
)

install(
    TARGETS stencil
    EXPORT stencilTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/stencil
)

install(FILES
    cmake/stencilTargets.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/stencilConfig.cmake
    DESTINATION share/stencil
)

if(STENCIL_INSTALL_BUILDTOOLS)
    if (TARGET lexyacc)
        install(TARGETS lexyacc EXPORT lexyaccTargets)
    elseif(EXISTS "${LEXYACC_EXECUTABLE}")
        install(FILES "${LEXYACC_EXECUTABLE}" DESTINATION bin)
    endif()

    install(FILES
        ${CMAKE_CURRENT_LIST_DIR}/external/lexyacc/FlexLexer.h
        ${CMAKE_CURRENT_LIST_DIR}/external/lexyacc/LexYaccConfig.cmake
        DESTINATION share/stencil
    )
endif()

#install(EXPORT stencilTargets
#    NAMESPACE stencil::
#    DESTINATION share/stencil
#)

#
# CPACK
#
set (CPACK_GENERATOR ZIP)
set (CPACK_PACKAGE_NAME                 ${PROJECT_NAME})
set (CPACK_PACKAGE_VENDOR               "Ankur Verma")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY  "Stencil Build Tools")
set (CPACK_PACKAGE_VERSION              ${PROJECT_VERSION})

include(CPack)
