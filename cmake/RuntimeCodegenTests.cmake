include(${CMAKE_CURRENT_LIST_DIR}/stencilTargets.cmake)
set(stencil_INCLUDE_PATH "${CMAKE_CURRENT_LIST_DIR}/../include" CACHE PATH "Stencil include path")

find_package(dtl REQUIRED MODULE)
find_package(Catch2 REQUIRED)

find_package(EmbedResource REQUIRED)

file(GLOB resfiles "${CMAKE_CURRENT_LIST_DIR}/../tests/testdata/*")
file(GLOB pidlfiles "${CMAKE_CURRENT_LIST_DIR}/../tests/*.pidl")

add_stencil_library(TARGET codegen IDLS ${pidlfiles})
add_resource_library(TARGET testdata RESOURCES ${resfiles} ${pidlfiles})

add_executable(codegen_runtime_tests
    "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_Codegen_CLOpts.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_Codegen_Database.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_Codegen_Json.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_DataRecorderPlayer.cpp"
    # TODO1 "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_ObservableOptionalProps.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_Transactions.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_Codegen_Catalog.cpp"
    # TODO1 "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_WebService_Autogen.cpp"
)

target_include_directories(codegen_runtime_tests PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../tests" ${DTL_INCLUDE_DIRS})
target_link_libraries(codegen_runtime_tests PRIVATE codegen testdata Catch2::Catch2 Catch2::Catch2WithMain)
add_test(NAME codegen_runtime_tests COMMAND codegen_runtime_tests)
