include_guard()

if (NOT TARGET TestCommon)
    include(${CMAKE_CURRENT_LIST_DIR}/modules/dtl.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/modules/Catch2.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/modules/EmbedResource.cmake)

    add_library(TestCommon INTERFACE)
    target_sources(TestCommon INTERFACE ${CMAKE_CURRENT_LIST_DIR}/TestUtils.h)
    target_include_directories(TestCommon INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
    target_link_libraries(TestCommon INTERFACE dtl::dtl Catch2::Catch2WithMain)
    if (MSVC)
        target_compile_options(TestCommon INTERFACE "/wd4868") # Catch2: not enforce evaluation order in braced initializer list
    endif()
endif()