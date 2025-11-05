include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/stencilTargets.cmake)
set(stencil_INCLUDE_PATH "../../include" CACHE PATH "Stencil include path")
include(../../cmake/TestCommon.cmake)
include(../../cmake/modules/BoostBeast.cmake)

file(GLOB test_data_files CONFIGURE_DEPENDS "../testdata/*")
file(GLOB pidlfiles "../*.pidl")

add_stencil_library(gencode OBJECT IDLS ${pidlfiles})
add_resource_library(testdata OBJECT RESOURCE_COLLECTION_NAME testdata RESOURCES ${test_data_files} ${pidlfiles})
target_compile_definitions(testdata PUBLIC HAVE_EMBEDRESOURCE=1)
target_include_directories(testdata PUBLIC "${EMBEDRESOURCE_INCLUDE_DIR}")
add_executable(codegen_runtime_tests
    "Test_Codegen_CLOpts.cpp"
    "Test_Database.cpp"
    # TODO1 "Test_ObservableOptionalProps.cpp"
    "Test_Transactions.cpp"
    "Test_Codegen_EnumConstsInitializers.cpp"
    "Test_Interfaces.cpp"
    "Test_Bugs.cpp"
)
target_compile_definitions(codegen_runtime_tests PRIVATE HAVE_EMBEDRESOURCE=1)
target_link_libraries(codegen_runtime_tests PRIVATE gencode testdata TestCommon)
set(CTEST_TEST_TIMEOUT 40)
if (NOT CMAKE_CROSSCOMPILING) # TODO: OR (DEFINED CMAKE_CROSSCOMPILING_EMULATOR)
    add_test(NAME codegen_runtime_tests COMMAND codegen_runtime_tests)
endif()
