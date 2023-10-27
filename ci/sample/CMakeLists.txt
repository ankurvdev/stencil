cmake_minimum_required(VERSION 3.26)
if(NOT EXISTS "${VCPKG_ROOT}")
    message(FATAL_ERROR "Please run with -DVCPKG_ROOT=<vcpkg-root> ${VCPKG_ROOT} not found")
endif()

# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../cmake)
# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../cmake/modules)

# set(INIT_SUBMODULE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../external)

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/BuildEnv.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/../../external/lexyacc/LexYaccConfig.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/modules/EmbedResource.cmake)

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

find_package(stencil REQUIRED)
set(srcfile ${CMAKE_CURRENT_BINARY_DIR}/main.cpp)
file(WRITE ${srcfile} "")
add_executable(stencil_vcpkg_test ${srcfile})

add_library(WavefrontObj OBJECT
    WavefrontObj/WavefrontObj.flex.cpp
    WavefrontObj/WavefrontObj.yacc.cpp
    WavefrontObj/WavefrontObj.ly.h
    WavefrontObj/WavefrontObj.ly.impl.h
    WavefrontObj/WavefrontObj.yacc.h
    WavefrontObj/location.hh
    WavefrontObj/position.hh
    WavefrontObj/stack.hh
)
target_include_directories(WavefrontObj PRIVATE WavefrontObj)
# add_lexyacc_library(TARGET WavefrontObj LYFILE ${CMAKE_CURRENT_LIST_DIR}/WavefrontObj.ly)
target_include_directories(WavefrontObj PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
SupressWarningForTarget(WavefrontObj)
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
