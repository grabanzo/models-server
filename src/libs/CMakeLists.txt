# This file serves as an entry point to build all libraries within this directory.

add_subdirectory(core)
add_subdirectory(framework)
add_subdirectory(http)

# An INTERFACE library that bundles all core libraries together.
# Executables can link against this single target to get all of them.
add_library(libs INTERFACE)

target_link_libraries(libs INTERFACE
    models_server_core
    models_server_framework
    models_server_http
)
