cmake_minimum_required(VERSION 3.12)

include(BuildEnv.cmake)

project(stencil VERSION 0.0.1)

include(CMakePackageConfigHelpers)

option(BUILD_TEST "Build Test" ON)
option(STENCIL_INSTALL_BUILDTOOLS "Install Extra Build Tools with install" ON)

set(CMAKE_CXX_STANDARD 20)
EnableStrictCompilation()

option(YAML_CPP_BUILD_CONTRIB "Enable yaml-cpp contrib in library" OFF)
option(YAML_CPP_BUILD_TOOLS "Enable parse tools" OFF)
option(YAML_CPP_BUILD_TESTS "Test" OFF)
option(YAML_BUILD_SHARED_LIBS "Shared" OFF)
add_subdirectory(external/yaml-cpp)

SupressWarningForTarget(yaml-cpp)

set(LexYacc_DIR          ${CMAKE_CURRENT_LIST_DIR}/external/lexyacc)
set(EmbedResource_DIR    ${CMAKE_CURRENT_LIST_DIR}/external/embedresource)

add_library(libstencil STATIC src/ModelGenerator.cpp src/Thrift.cpp)
find_package(LexYacc)
find_package(EmbedResource)

build_lexyacc()
build_embedresource()

file(GLOB resfiles src/templates/*)
target_add_resource(libstencil templates ${resfiles})

file(GLOB resfiles include/stencil/*)
if (NOT BUILD_TEST)
target_add_resource(libstencil commonheaders ${resfiles})
endif()

target_add_lexyacc(libstencil src/Thrift.ly)
target_link_libraries(libstencil PRIVATE yaml-cpp::yaml-cpp)

target_include_directories(libstencil PUBLIC src external/tinyxml2)
target_sources(libstencil PRIVATE external/tinyxml2/tinyxml2.cpp)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    # wd4365 signed unsigned mismatch
    set_source_files_properties(external/tinyxml2/tinyxml2.cpp PROPERTIES COMPILE_FLAGS "-wd4365 -wd4668")
endif()
add_executable(stencil src/main.cpp)

file(GLOB public_headers ${CMAKE_CURRENT_LIST_DIR}/include/stencil/*)

set_target_properties(stencil PROPERTIES PUBLIC_HEADER "${public_headers}")

target_link_libraries(stencil PRIVATE libstencil)

if (BUILD_TEST)
    enable_testing()

    file(GLOB resfiles tests/testdata/*)
    file(GLOB pidlfiles tests/*.pidl)

    add_executable(codegen_tests
        tests/Test_ThriftGenerator.cpp
    )
    add_test(NAME codegen_tests COMMAND codegen_tests)

    target_include_directories(codegen_tests PUBLIC src external/catch2)
    target_include_directories(codegen_tests PUBLIC src external)
    target_include_directories(codegen_tests PUBLIC include)
    target_link_libraries(codegen_tests PRIVATE libstencil)
    target_add_resource(codegen_tests testdata ${resfiles} ${pidlfiles})

    add_executable(tests
        tests/Test_CLOpts.cpp
        tests/Test_CommandLineArgsReaderHandler.cpp
        tests/Test_Database2.cpp
        tests/Test_Database.cpp
        tests/Test_DataHandlerJson.cpp
        tests/Test_WebService_Handwritten.cpp
        tests/Test_WebService_Autogen.cpp
        tests/Test_ObservableOptionalProps.cpp
        tests/Test_DataRecorderPlayer.cpp
    )

    target_add_resource(tests testdata ${resfiles} ${pidlfiles})
    target_include_directories(tests PRIVATE src external/catch2)
    target_include_directories(tests PRIVATE src external)
    target_include_directories(tests PRIVATE include)


    foreach(f ${pidlfiles})
        get_filename_component(fname "${f}" NAME)
        list(APPEND outputs "${CMAKE_CURRENT_BINARY_DIR}/${fname}.h")
        list(APPEND inputs "${f}")
    endforeach()
    add_custom_command(OUTPUT ${outputs}
                COMMAND stencil --outdir=${CMAKE_CURRENT_BINARY_DIR} ${inputs}
                DEPENDS stencil ${inputs})
    add_custom_target(codegen DEPENDS ${outputs})
    add_dependencies(tests codegen)
    target_include_directories(tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR} include)
    target_compile_definitions(tests PRIVATE USE_NLOHMANN_JSON)
    target_compile_definitions(tests PRIVATE USE_CPP_HTTPLIB)
    target_link_libraries(tests PRIVATE libstencil)
    find_package (Threads REQUIRED)
    target_link_libraries (tests PRIVATE Threads::Threads)
    add_test(NAME tests COMMAND tests)
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
