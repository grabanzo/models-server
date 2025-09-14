# Este fichero simplemente añade todos los módulos que encuentre como subdirectorios.
add_subdirectory(health)

# Conditionally add the dummy module for testing purposes
if(BUILD_DUMMY_MODULE)
    message(STATUS "Dummy module is enabled.")
    add_subdirectory(dummy)
else()
    message(STATUS "Dummy module is disabled.")
endif()
