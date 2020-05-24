cmake_minimum_required(VERSION 3.12)
project(stencil VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
if(NOT EXISTS ${VCPKG_ROOT})
	message(FATAL_ERROR "Please run with -DVCPKG_ROOT=<vcpkg-root>")
endif()
if (WIN32)
	set(VCPKG_TARGET_TRIPLET x86-windows CACHE STRING "" FORCE)
else()
	set(VCPKG_TARGET_TRIPLET x64-linux CACHE STRING "" FORCE)
endif()
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

message(STATUS "Using Triplet ${VCPKG_TARGET_TRIPLET}")
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
	for (auto const& r : resourceCollection)
        {
		std::cout << r.string();
	}	
	return 0; 
}"


)

