find_package(EmbedResource CONFIG QUIET)

if (NOT EmbedResource_FOUND)
    init_submodule(embedresource)
    include(${INIT_SUBMODULE_DIRECTORY}/embedresource/cmake/EmbedResourceTargets.cmake) # For add_subdirectory mode
endif()
