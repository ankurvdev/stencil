if (NOT TARGET TestCommon)
    find_package(dtl REQUIRED MODULE)
    find_package(Catch2 REQUIRED)
    find_package(EmbedResource REQUIRED)

    add_library(TestCommon INTERFACE)
    target_sources(TestCommon INTERFACE ${CMAKE_CURRENT_LIST_DIR}/TestUtils.h)
    target_include_directories(TestCommon INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
    target_link_libraries(TestCommon INTERFACE dtl::dtl Catch2::Catch2WithMain)
    if (MSVC)
        target_compile_options(Catch2::Catch2WithMain INTERFACE "/wd4868") #not enforce evaluation order in braced initializer list
    endif()
endif()