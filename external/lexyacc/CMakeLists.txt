cmake_minimum_required(VERSION 3.26)
include(CMakePackageConfigHelpers)
set(CMAKE_CXX_STANDARD 17)
option(LEXYACC_SKIP_INSTALL "Do not install lexyacc" OFF)

project(lexyacc VERSION 0.0.1)

add_executable(lexyacc LexYacc.cpp)
if (NOT LEXYACC_SKIP_INSTALL)
    install(
        TARGETS lexyacc
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif()