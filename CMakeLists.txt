cmake_minimum_required(VERSION 3.12)

include(BuildEnv.cmake)

project(stencil VERSION 0.0.3)

include(CMakePackageConfigHelpers)

option(BUILD_TESTING "Build Test" ON)
option(STENCIL_INSTALL_BUILDTOOLS "Install Extra Build Tools with install" ON)

set(CMAKE_CXX_STANDARD 20)
EnableStrictCompilation()

find_package (Threads REQUIRED)

set(LexYacc_DIR       ${CMAKE_CURRENT_LIST_DIR}/external/lexyacc)
set(EmbedResource_DIR ${CMAKE_CURRENT_LIST_DIR}/external/embedresource)

find_package(LexYacc)
find_package(EmbedResource)

find_package(toml11 QUIET)
if (NOT toml11_FOUND)
    init_submodule(external/toml11)
    add_library(toml11 INTERFACE)
    add_library(toml11::toml11 ALIAS toml11)
    target_include_directories(toml11 INTERFACE external/toml11)
endif()

find_package(tsl-ordered-map QUIET)
if (NOT tsl-ordered-map_FOUND)
    init_submodule(external/ordered-map)
    add_library(tsl_ordered_map INTERFACE)
    add_library(tsl::ordered_map ALIAS tsl_ordered_map)
    target_include_directories(tsl_ordered_map INTERFACE external/ordered-map/include)
endif()

find_package(tinyxml2 QUIET)
if (NOT tinyxml2_FOUND)
    init_submodule(external/tinyxml2)
    add_library(tinyxml2 external/tinyxml2/tinyxml2.cpp external/tinyxml2/tinyxml2.h)
    target_compile_definitions(tinyxml2 PUBLIC $<$<CONFIG:Debug>:TINYXML2_DEBUG>)
    target_include_directories(tinyxml2 PUBLIC external/tinyxml2)
    add_library(tinyxml2::tinyxml2 ALIAS tinyxml2)
    SupressWarningForTarget(tinyxml2)
endif()

find_package(cxxopts QUIET)
if (NOT cxxopts_FOUND)
    init_submodule(external/cxxopts)
    add_library(cxxopts INTERFACE)
    add_library(cxxopts::cxxopts ALIAS cxxopts)
    target_include_directories(cxxopts INTERFACE external/cxxopts/include)
endif()


find_package(stduuid QUIET)
if (NOT stduuid_FOUND)
    init_submodule(external/stduuid)
    add_library(stduuid INTERFACE)
    add_library(stduuid::stduuid ALIAS stduuid)
    target_include_directories(stduuid INTERFACE external/stduuid/include)
endif()


find_package(fmt QUIET)
if (NOT fmt_FOUND)
    init_submodule(external/fmt)
    add_subdirectory(external/fmt EXCLUDE_FROM_ALL fmt)
    SupressWarningForTarget(fmt)
endif()


find_package(date QUIET)
if (NOT date_FOUND)
    init_submodule(external/date)
    add_subdirectory(external/date EXCLUDE_FROM_ALL date)
endif()


find_package(RapidJSON QUIET)
if (NOT RapidJSON_FOUND)
    init_submodule(external/rapidjson)
    add_subdirectory(external/rapidjson EXCLUDE_FROM_ALL rapidjson)
    SupressWarningForTarget(rapidjson)
    target_include_directories(rapidjson PUBLIC USE_RAPIDJSON=1)
endif()


find_package(simdjson QUIET)
if (NOT simdjson_FOUND)
    init_submodule(external/simdjson)
    add_library(simdjson INTERFACE)
endif()


if (BUILD_TESTING)
    enable_testing()

    find_package(catch2 QUIET)
    if (NOT catch2_FOUND)
        init_submodule(external/Catch2)
        add_library(catch2 INTERFACE)
        add_library(Catch2::Catch2 ALIAS catch2)
        target_include_directories(catch2 INTERFACE external/Catch2/single_include)
    endif()

    find_path(DTL_INCLUDE_DIRS "dtl/Diff.hpp")
    if (NOT EXISTS "${DTL_INCLUDE_DIRS}")
        init_submodule(external/dtl)
        set(DTL_INCLUDE_DIRS external/dtl)
    endif()
    add_library(dtl INTERFACE)
    add_library(dtl::dtl ALIAS dtl)
    target_include_directories(dtl INTERFACE "${DTL_INCLUDE_DIRS}")

    find_path(CPP_HTTPLIB_INCLUDE_DIRS "httplib.h")
    if (NOT EXISTS "${CPP_HTTPLIB_INCLUDE_DIRS}")
        init_submodule(external/cpp-httplib)
        set(CPP_HTTPLIB_INCLUDE_DIRS "external/cpp-httplib")
    endif()
    add_library(cpp_httplib INTERFACE)
    add_library(cpp_httplib::cpp_httplib ALIAS cpp_httplib)
    target_include_directories(cpp_httplib INTERFACE "${CPP_HTTPLIB_INCLUDE_DIRS}")
endif()

build_lexyacc()
build_embedresource()

file(GLOB resfiles src/templates/*)

add_library(libstencil OBJECT src/ModelGenerator.cpp src/Thrift.cpp)
target_add_lexyacc(libstencil src/Thrift.ly)
target_add_resource(libstencil templates ${resfiles})

file(GLOB resfiles include/stencil/*)
if (NOT BUILD_TESTING)
    target_add_resource(libstencil commonheaders ${resfiles})
endif()

target_link_libraries(libstencil PRIVATE toml11::toml11 tinyxml2::tinyxml2 tsl::ordered_map)
target_link_libraries(libstencil PUBLIC  fmt::fmt stduuid date::date)

target_include_directories(libstencil PUBLIC src include/stencil)
add_executable(stencil src/main.cpp)

file(GLOB public_headers ${CMAKE_CURRENT_LIST_DIR}/include/stencil/*)
find_file(UUIDHEADER REQUIRED NAMES uuid.h HINTS external/stduuid/include)
list(APPEND public_headers "${UUIDHEADER}")


set_target_properties(stencil PROPERTIES PUBLIC_HEADER "${public_headers}")

target_link_libraries(stencil PRIVATE libstencil cxxopts::cxxopts)

if (BUILD_TESTING)
    enable_testing()

    file(GLOB resfiles tests/testdata/*)
    file(GLOB pidlfiles tests/*.pidl)

    add_executable(codegen_tests
        tests/Codegen/Test_ThriftGenerator.cpp
    )
    add_test(NAME codegen_tests COMMAND codegen_tests)
    target_include_directories(codegen_tests PRIVATE src include tests)
    target_link_libraries(codegen_tests PRIVATE libstencil dtl::dtl Catch2::Catch2)
    target_add_resource(codegen_tests testdata ${resfiles} ${pidlfiles})
    
    foreach(f ${pidlfiles})
        get_filename_component(fname "${f}" NAME)
        list(APPEND outputs "${CMAKE_CURRENT_BINARY_DIR}/${fname}.h")
        list(APPEND inputs "${f}")
    endforeach()
    add_custom_command(OUTPUT ${outputs}
                COMMAND stencil --outdir=${CMAKE_CURRENT_BINARY_DIR} ${inputs}
                DEPENDS stencil ${inputs})
    add_custom_target(codegen DEPENDS ${outputs})

    add_executable(runtime_tests
        tests/HandwrittenRuntime/Test_Handwritten_CLOpts.cpp
        tests/HandwrittenRuntime/Test_Handwritten_Json.cpp
        tests/HandwrittenRuntime/Test_Handwritten_Binary.cpp
        tests/HandwrittenRuntime/Test_Database.cpp
        tests/HandwrittenRuntime/Test_Database2.cpp
        #tests/HandwrittenRuntime/Test_WebService_Handwritten.cpp
    )

    add_dependencies(runtime_tests codegen)
    target_include_directories(runtime_tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR} include)
    target_compile_definitions(runtime_tests PRIVATE USE_NLOHMANN_JSON USE_CPP_HTTPLIB)
    target_link_libraries(runtime_tests PRIVATE libstencil)
    target_link_libraries(runtime_tests PRIVATE dtl::dtl rapidjson Catch2::Catch2 cpp_httplib::cpp_httplib fmt)
    target_link_libraries (runtime_tests PRIVATE Threads::Threads)
    add_test(NAME runtime_tests COMMAND runtime_tests)

    target_add_resource(runtime_tests testdata ${resfiles} ${pidlfiles})
    target_include_directories(runtime_tests PRIVATE src include tests)
    add_executable(codegen_runtime_tests
        tests/CodegenRuntime/Test_Codegen_CLOpts.cpp
        tests/CodegenRuntime/Test_Codegen_Json.cpp
        tests/CodegenRuntime/Test_DataRecorderPlayer.cpp
        #tests/CodegenRuntime/Test_ObservableOptionalProps.cpp
        tests/CodegenRuntime/Test_Transactions.cpp
        #tests/CodegenRuntime/Test_WebService_Autogen.cpp
    )
    add_dependencies(codegen_runtime_tests codegen)
    target_add_resource(codegen_runtime_tests testdata ${resfiles} ${pidlfiles})

    target_link_libraries(codegen_runtime_tests PRIVATE libstencil dtl::dtl Catch2::Catch2 rapidjson cpp_httplib::cpp_httplib fmt)
    target_include_directories(codegen_runtime_tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR} src include tests)
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
    install(TARGETS embedresource EXPORT embedresourceTargets)
    install(TARGETS lexyacc       EXPORT lexyaccTargets)
    install(FILES
        ${CMAKE_CURRENT_LIST_DIR}/external/lexyacc/FlexLexer.h
        ${CMAKE_CURRENT_LIST_DIR}/external/lexyacc/LexYaccConfig.cmake
        ${CMAKE_CURRENT_LIST_DIR}/external/embedresource/EmbedResourceConfig.cmake
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
