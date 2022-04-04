cmake_minimum_required(VERSION 3.12)
if(NOT EXISTS "${VCPKG_ROOT}")
    message(FATAL_ERROR "Please run with -DVCPKG_ROOT=<vcpkg-root> ${VCPKG_ROOT} not found")
endif()

if (NOT DEFINED VCPKG_TARGET_TRIPLET)
    if (WIN32)
        set(VCPKG_TARGET_TRIPLET x86-windows)
    else()
        set(VCPKG_TARGET_TRIPLET x64-linux)
    endif()
endif()

if (NOT DEFINED VCPKG_HOST_TRIPLET)
    set(VCPKG_HOST_TRIPLET ${VCPKG_TARGET_TRIPLET})
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../../BuildEnv.cmake)
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
message(STATUS " 
    CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}
    CMAKE_LIBRARY_PATH: ${CMAKE_LIBRARY_PATH}
    CMAKE_FIND_ROOT_PATH: ${CMAKE_FIND_ROOT_PATH}
    VCPKG_CMAKE_FIND_ROOT_PATH: ${VCPKG_CMAKE_FIND_ROOT_PATH}
    VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}
    VCPKG_HOST_TRIPLET: ${VCPKG_HOST_TRIPLET}
")

project(stencil-test VERSION 0.0.1)

EnableStrictCompilation()
set(CMAKE_CXX_STANDARD 20)
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

find_package(stencil REQUIRED)

file(GLOB pidlfiles ${CMAKE_CURRENT_LIST_DIR}/../../tests/*.pidl)
if ( "${pidlfiles}" STREQUAL "")
    message(FATAL_ERROR "Cannot find any pidl files to include in ${CMAKE_CURRENT_LIST_DIR}/../../tests")
endif()

set(srcfile ${CMAKE_CURRENT_BINARY_DIR}/main.cpp)
file(WRITE ${srcfile} "")
add_executable(stencil_vcpkg_test ${srcfile})
foreach(f ${pidlfiles})
    get_filename_component(fname ${f} NAME)
    file(APPEND ${srcfile} "#include <${fname}.h>\n")
    target_add_stencil(stencil_vcpkg_test ${f})
endforeach()

target_add_lexyacc(stencil_vcpkg_test ${CMAKE_CURRENT_LIST_DIR}/WavefrontObj.ly)
target_include_directories(stencil_vcpkg_test PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_add_resource(stencil_vcpkg_test testdata ${pidlfiles})

file(APPEND ${srcfile} "
#include<WavefrontObj.ly.h>
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

