project(health_module)

add_library(${PROJECT_NAME} STATIC
    HealthModule.cpp
    routes/HealthRoutes.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# El módulo necesita acceso a las interfaces del framework y a los tipos http
target_link_libraries(${PROJECT_NAME} PUBLIC
    models_server_framework
    models_server_http
)
