# tests/CMakeLists.txt

# --- Dependencias de Test ---
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0 # Usamos una versión estable y reciente
)
FetchContent_MakeAvailable(googletest)

# --- Definición del Ejecutable de Pruebas ---
add_executable(run_tests
    # Aquí iremos añadiendo nuestros ficheros de test
    test_config.cpp
)

target_sources(run_tests PRIVATE
    test_config.cpp
    test_routes.cpp
)

# --- Enlazado ---
# Enlazamos las mismas bibliotecas y ficheros fuente que nuestro ejecutable principal
# para poder probarlos.
target_link_libraries(run_tests
    PRIVATE
    # Librerías de nuestro proyecto
    http_server
    inference_engine_mock
    common_utils
    # Fuentes directas del ejecutable
    ${CMAKE_SOURCE_DIR}/src/apps/mserver/Config.cpp
    ${CMAKE_SOURCE_DIR}/src/apps/mserver/MasterProcess.cpp
    # Dependencias externas
    yaml-cpp
    dotenv
    GTest::gtest_main # La librería principal de Google Test
)

# Añadimos los directorios de inclusión necesarios para que los tests encuentren los headers.
target_include_directories(run_tests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/libs
    ${CMAKE_SOURCE_DIR}/src/apps/mserver
)


# --- Integración con CTest ---
include(GoogleTest)
gtest_discover_tests(run_tests)
