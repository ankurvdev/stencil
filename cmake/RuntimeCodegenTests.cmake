include(${CMAKE_CURRENT_LIST_DIR}/stencilTargets.cmake)
set(stencil_INCLUDE_PATH "${CMAKE_CURRENT_LIST_DIR}/../include" CACHE PATH "Stencil include path")

find_package(TestCommon REQUIRED MODULE)
find_package(CppHttpLib REQUIRED MODULE)
find_package(EmbedResource REQUIRED MODULE)

file(GLOB test_data_files CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/../tests/testdata/*")
file(GLOB pidlfiles "${CMAKE_CURRENT_LIST_DIR}/../tests/*.pidl")

add_stencil_library(TARGET codegen IDLS ${pidlfiles})
add_resource_library(TARGET testdata RESOURCES ${test_data_files} ${pidlfiles})

add_executable(codegen_runtime_tests
    "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_Codegen_CLOpts.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_Database.cpp"
    # TODO1 "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_ObservableOptionalProps.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_Transactions.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_Codegen_EnumConstsInitializers.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../tests/CodegenRuntime/Test_Interfaces.cpp"
)
target_compile_definitions(codegen_runtime_tests PRIVATE HAVE_EMBEDRESOURCE=1)
target_link_libraries(codegen_runtime_tests PRIVATE codegen testdata TestCommon websvc CppHttpLib)
add_test(NAME codegen_runtime_tests COMMAND codegen_runtime_tests)
