project(mserver_core)

add_library(${PROJECT_NAME}
    STATIC
        src/app_config.cpp
        src/app.cpp
        src/core_config.cpp
        src/thread_pool_config.cpp
        src/thread_pool.cpp
        src/worker.cpp
        src/worker_http_config.cpp
        src/worker_http.cpp
        src/logger.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yaml-cpp::yaml-cpp
        spdlog::spdlog
        httplib::httplib
        nlohmann_json::nlohmann_json
        dotenv
        cxxopts::cxxopts
)
