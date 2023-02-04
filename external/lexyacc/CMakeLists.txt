cmake_minimum_required(VERSION 3.12)
include(CMakePackageConfigHelpers)

project(lexyacc VERSION 0.0.1)

add_executable(lexyacc LexYacc.cpp)
install(
    TARGETS lexyacc
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
