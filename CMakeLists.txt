cmake_minimum_required(VERSION 3.26)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
set(INIT_SUBMODULE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/external)

include(BuildEnv)
include(FetchContent)
file(READ ci/vcpkg-additional-ports/stencil/vcpkg.json VCPKG_MANIFEST_INFO)
string(JSON PROJECT_VERSION GET "${VCPKG_MANIFEST_INFO}" "version")
project(stencil VERSION "${PROJECT_VERSION}")
if (DEFINED Quick_DIR)
    find_package(Quick)
    QuickInit()
endif()

include(CMakePackageConfigHelpers)

option(BUILD_TESTING "Build Test" ON)
option(STENCIL_INSTALL_BUILDTOOLS "Install Extra Build Tools with install" OFF)

set(CMAKE_CXX_STANDARD 20)
EnableStrictCompilation()


find_package(Threads REQUIRED)

file(GLOB public_headers ${CMAKE_CURRENT_LIST_DIR}/include/stencil/*)

include(cmake/modules/cxxopts.cmake)
include(cmake/modules/date.cmake)
include(cmake/modules/EmbedResource.cmake)
include(cmake/modules/fmt.cmake)
include(cmake/modules/BoostBeast.cmake)
include(cmake/modules/LexYacc.cmake)
include(cmake/modules/RapidJSON.cmake)
include(cmake/modules/stduuid.cmake)
include(cmake/modules/tinyxml2.cmake)
include(cmake/modules/toml11.cmake)
include(cmake/modules/tsl-ordered-map.cmake)

include(cmake/CommonMacros.cmake)

list(APPEND public_headers "${COMMONMACROS_INCLUDE_FILE}")

if (BUILD_TESTING)
    enable_testing()
    include(cmake/modules/Catch2.cmake)
endif()

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
target_include_directories(libstencil PUBLIC src include)
add_lexyacc_library(TARGET Thrift LYFILE src/Thrift.ly SOURCES src/Thrift.cpp)
target_include_directories(Thrift PUBLIC src include)
target_link_libraries(Thrift PUBLIC CommonMacros fmt::fmt-header-only)
if (MSVC)
    # TOML11 has a nasty usage of insecure apis
    set_source_files_properties("src/ModelGenerator.cpp" PROPERTIES COMPILE_FLAGS "/D_CRT_SECURE_NO_WARNINGS")
endif()
target_link_libraries(libstencil PUBLIC Thrift)
target_add_resource(libstencil RESOURCE_COLLECTION_NAME templates RESOURCES ${resfiles})
list(APPEND resfiles ${public_headers})
if (NOT BUILD_TESTING)
    target_add_resource(libstencil RESOURCE_COLLECTION_NAME commonheaders RESOURCES ${resfiles})
endif()

target_link_libraries(libstencil PUBLIC
    toml11::toml11
    tinyxml2::tinyxml2
    tsl::ordered_map
    fmt::fmt-header-only
    CommonMacros
)

set(stencil_INCLUDE_PATH "${CMAKE_CURRENT_LIST_DIR}/include" CACHE PATH "Stencil include path")
# For add_subdirectory mode
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
    include(cmake/TestCommon.cmake)
    file(GLOB resfiles CONFIGURE_DEPENDS tests/testdata/*)
    file(GLOB pidlfiles tests/*.pidl)

    add_executable(codegen_tests
        tests/Codegen/Test_ThriftGenerator.cpp
    )
    add_test(NAME codegen_tests COMMAND codegen_tests)
    target_include_directories(codegen_tests PRIVATE src include tests)
    target_link_libraries(codegen_tests PRIVATE libstencil Thrift TestCommon)
    target_add_resource(codegen_tests RESOURCE_COLLECTION_NAME testdata RESOURCES ${resfiles} ${pidlfiles})
    target_compile_definitions(codegen_tests PRIVATE HAVE_EMBEDRESOURCE=1)
    add_executable(runtime_tests
        tests/HandwrittenRuntime/Test_Handwritten_CLOpts.cpp
        tests/HandwrittenRuntime/Test_Handwritten_Json.cpp
        tests/HandwrittenRuntime/Test_Handwritten_Binary.cpp
    )

    target_compile_definitions(runtime_tests PRIVATE HAVE_NLOHMANN_JSON)
    target_link_libraries(runtime_tests PRIVATE stencil_runtime)
    target_link_libraries(runtime_tests PRIVATE TestCommon httplib::httplib stencil::runtime)
    target_link_libraries (runtime_tests PRIVATE Threads::Threads)
    add_test(NAME runtime_tests COMMAND runtime_tests)

    target_add_resource(runtime_tests RESOURCE_COLLECTION_NAME testdata RESOURCES ${resfiles})
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
    cmake/FindOrBuildTool.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/stencilConfig.cmake
    DESTINATION share/stencil
)

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
