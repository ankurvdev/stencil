cmake_minimum_required(VERSION 3.12)
project(stencil VERSION 0.0.1)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 17)

set(LexYacc_DIR          ${CMAKE_CURRENT_LIST_DIR}/external/lexyacc)
set(EmbedResource_DIR    ${CMAKE_CURRENT_LIST_DIR}/external/embedresource)


add_library(libstencil STATIC src/ModelGenerator.cpp src/Thrift.cpp)
find_package(LexYacc)
find_package(EmbedResource)

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

target_include_directories(libstencil PUBLIC src external/tinyxml2)
target_sources(libstencil PRIVATE external/tinyxml2/tinyxml2.cpp)
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
    PUBLIC_HEADER DESTINATION include
)

install(FILES
    cmake/stencilTargets.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/stencilConfig.cmake
    DESTINATION share/stencil/cmake
)

if(STENCIL_INSTALL_BUILDTOOLS)
    install(TARGETS embedresource EXPORT embedresourceTargets)
    install(TARGETS lexyacc       EXPORT lexyaccTargets)
    install(FILES
        ${CMAKE_CURRENT_LIST_DIR}/external/lexyacc/LexYaccConfig.cmake
        ${CMAKE_CURRENT_LIST_DIR}/external/embedresource/EmbedResourceConfig.cmake
        DESTINATION share/stencil/cmake
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
