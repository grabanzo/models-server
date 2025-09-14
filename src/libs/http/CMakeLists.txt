project(models_server_http)

add_library(${PROJECT_NAME} STATIC
    src/HttpServer.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# The http library depends on our framework and core libraries, plus external ones.
target_link_libraries(${PROJECT_NAME} PUBLIC
    models_server_core
    models_server_framework
    httplib::httplib
)
