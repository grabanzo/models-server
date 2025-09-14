project(mserver_modules_dummy)

add_library(${PROJECT_NAME} 
    STATIC
        src/dummy_module.cpp
        src/dummy_routes.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 4. Enlaza las dependencias necesarias. Como mínimo, necesitarás 'framework' y 'http'.
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    mserver_core
)
