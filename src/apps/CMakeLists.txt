project(apps)

set(MSERVER_NAME mserver)

# --- Ejecutable Principal ---
add_executable(${MSERVER_NAME}
    mserver/main.cpp
    mserver/MasterProcess.cpp
)
target_link_libraries(${MSERVER_NAME} 
    PRIVATE
    libs 
    health_module
    figcone::figcone
)

if(BUILD_DUMMY_MODULE)
    target_link_libraries(${MSERVER_NAME} PRIVATE dummy_module)
endif()

# En macOS, httplib necesita CoreFoundation y CoreServices para la resolución de DNS.
if(APPLE)
    target_link_libraries(${MSERVER_NAME}
        PRIVATE
        "-framework CoreFoundation"
        "-framework CoreServices"
    )
endif()
