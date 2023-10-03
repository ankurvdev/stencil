cmake_minimum_required(VERSION 3.12)
if(NOT EXISTS "${VCPKG_ROOT}")
    message(FATAL_ERROR "Please run with -DVCPKG_ROOT=<vcpkg-root> ${VCPKG_ROOT} not found")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../cmake/modules)

set(INIT_SUBMODULE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../external)

include(BuildEnv)
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
message(STATUS " 
    CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}
    CMAKE_LIBRARY_PATH: ${CMAKE_LIBRARY_PATH}
    CMAKE_FIND_ROOT_PATH: ${CMAKE_FIND_ROOT_PATH}
    VCPKG_CMAKE_FIND_ROOT_PATH: ${VCPKG_CMAKE_FIND_ROOT_PATH}
    VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}
    VCPKG_HOST_TRIPLET: ${VCPKG_HOST_TRIPLET}
")
set(UUID_USING_CXX20_SPAN CACHE BOOL ON "UUID_USING_CXX20_SPAN")

project(stencil-test VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
EnableStrictCompilation()
#set(CMAKE_FIND_DEBUG_MODE TRUE)

if (NOT "${VCPKG_HOST_TRIPLET}" STREQUAL "${VCPKG_TARGET_TRIPLET}")
    find_program(STENCIL_HOST_EXECUTABLE stencil REQUIRED NO_DEFAULT_PATH PATHS ${VCPKG_ROOT}/installed/${VCPKG_HOST_TRIPLET}/tools)
    find_program(LEXYACC_HOST_EXECUTABLE lexyacc REQUIRED NO_DEFAULT_PATH PATHS ${VCPKG_ROOT}/installed/${VCPKG_HOST_TRIPLET}/tools)
    find_program(EMBEDRES_HOST_EXECUTABLE embedresource REQUIRED NO_DEFAULT_PATH PATHS ${VCPKG_ROOT}/installed/${VCPKG_HOST_TRIPLET}/tools)
    set(STENCIL_EXECUTABLE "${STENCIL_HOST_EXECUTABLE}" CACHE PATH "" FORCE )
    set(EMBEDRESOURCE_EXECUTABLE "${EMBEDRES_HOST_EXECUTABLE}" CACHE PATH "" FORCE )
    set(LEXYACC_EXECUTABLE "${LEXYACC_HOST_EXECUTABLE}" CACHE PATH "" FORCE )
    message(STATUS " 
        STENCIL_EXECUTABLE: ${STENCIL_EXECUTABLE}
        EMBEDRESOURCE_EXECUTABLE: ${EMBEDRESOURCE_EXECUTABLE}
        LEXYACC_EXECUTABLE: ${LEXYACC_EXECUTABLE}
    ")
endif()

find_package(EmbedResource)
find_package(stencil REQUIRED)
set(srcfile ${CMAKE_CURRENT_BINARY_DIR}/main.cpp)
file(WRITE ${srcfile} "")
add_executable(stencil_vcpkg_test ${srcfile})

add_lexyacc_library(TARGET WavefrontObj LYFILE ${CMAKE_CURRENT_LIST_DIR}/WavefrontObj.ly)
target_include_directories(WavefrontObj PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
target_include_directories(stencil_vcpkg_test PRIVATE "${CMAKE_CURRENT_LIST_DIR}")

file(APPEND ${srcfile} "
#pragma warning(push, 3)
#pragma warning(disable : 5262) /*xlocale(2010,13): implicit fall-through occurs here*/
#include<WavefrontObj.h>
#include<EmbeddedResource.h>
#include <iostream>
#pragma warning(pop)
DECLARE_RESOURCE_COLLECTION(testdata);
int main() { 
    auto resourceCollection = LOAD_RESOURCE_COLLECTION(testdata);
    for (auto const r : resourceCollection)
    {
        std::cout << r.string();
    }
    return 0;
}
"
)

enable_testing()
add_test(NAME stencil_vcpkg_test COMMAND stencil_vcpkg_test)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../tests/CodegenRuntime CodegenRuntime)
target_link_libraries(stencil_vcpkg_test PRIVATE codegen testdata WavefrontObj)
