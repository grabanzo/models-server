project(mserver)

# --- Ejecutable Principal ---
add_executable(${PROJECT_NAME}
    main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        mserver_core 
        mserver_modules_health
    # health_module
    # figcone::figcone
)

# if(BUILD_DUMMY_MODULE)
#     target_link_libraries(${MSERVER_NAME} PRIVATE dummy_module)
# endif()

# En macOS, httplib necesita CoreFoundation y CoreServices para la resolución de DNS.
if(APPLE)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        "-framework CoreFoundation"
        "-framework CoreServices"
    )
endif()
