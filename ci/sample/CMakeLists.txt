cmake_minimum_required(VERSION 3.26)
if(NOT EXISTS "${VCPKG_ROOT}")
    message(FATAL_ERROR "Please run with -DVCPKG_ROOT=<vcpkg-root> ${VCPKG_ROOT} not found")
endif()
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/BuildEnv.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/modules/EmbedResource.cmake)

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

find_package(stencil REQUIRED)
set(srcfile ${CMAKE_CURRENT_BINARY_DIR}/main.cpp)
file(WRITE ${srcfile} "")
add_executable(stencil_vcpkg_test ${srcfile})
target_include_directories(stencil_vcpkg_test PRIVATE "${CMAKE_CURRENT_LIST_DIR}")

file(APPEND ${srcfile} "
#include<EmbeddedResource.h>
#include <iostream>
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
target_link_libraries(stencil_vcpkg_test PRIVATE gencode testdata)
