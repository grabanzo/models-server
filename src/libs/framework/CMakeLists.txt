project(models_server_framework)

# This library now has implementation files, so it becomes a STATIC library.
add_library(${PROJECT_NAME} STATIC
    src/ModuleRegistry.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    # Exposes the 'include' directory for #include <framework/IService.hpp>
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# The framework needs httplib to define interfaces like IRoute.
# Making it PUBLIC ensures that any target linking to framework also links to httplib.
target_link_libraries(models_server_framework
    PUBLIC
    httplib::httplib
)
