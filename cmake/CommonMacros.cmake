include_guard()

if (NOT TARGET CommonMacros)
    add_library(CommonMacros INTERFACE)
    set(COMMONMACROS_INCLUDE_FILE "${CMAKE_CURRENT_LIST_DIR}/CommonMacros.h")
    target_sources(CommonMacros INTERFACE "${COMMONMACROS_INCLUDE_FILE}")
    target_include_directories(CommonMacros INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
endif()