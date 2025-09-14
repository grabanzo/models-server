project(models_server_core)

add_library(${PROJECT_NAME} STATIC
    src/Logger.cpp
    src/ThreadPool.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    models_server_framework
    spdlog::spdlog
)
