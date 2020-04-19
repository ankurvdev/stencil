cmake_minimum_required(VERSION 3.12)
project(stencil)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 17)

set(LexYacc_DIR             ${CMAKE_CURRENT_LIST_DIR}/buildtools)
set(EmbeddedResource_DIR    ${CMAKE_CURRENT_LIST_DIR}/buildtools)

add_library(libstencil STATIC src/ModelGenerator.cpp src/Thrift.cpp)
find_package(LexYacc)
find_package(EmbeddedResource)

target_add_resource(libstencil templates 
    src/templates/1.typedef.common.txt
    src/templates/2.template.common.struct.cpp
    src/templates/3.template.common.struct.h
    src/templates/4.template.json.struct.h
    src/templates/5.template.sqlite.struct.cpp
    src/templates/6.template.sqlite.struct.h
)

target_add_lexyacc(libstencil src/Thrift.ly)
target_add_lexyacc(libstencil src/TypeDefinition.ly)

target_include_directories(libstencil PUBLIC src external)
target_sources(libstencil PRIVATE external/tinyxml2.cpp)
add_executable(stencil src/main.cpp)

file(GLOB public_headers ${CMAKE_CURRENT_LIST_DIR}/include/*)

set_target_properties(stencil PROPERTIES PUBLIC_HEADER "${public_headers}")

target_link_libraries(stencil PRIVATE libstencil)

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
)

install(FILES 
    cmake/stencilTargets.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/stencilConfig.cmake 
    DESTINATION share/stencil/cmake
)

#install(EXPORT stencilTargets
#    NAMESPACE stencil::
#    DESTINATION share/stencil
#)