cmake_minimum_required(VERSION 3.15)
project(ModelsServer)

# Options
option(BUILD_DUMMY_MODULE "Build the dummy module for testing purposes" OFF)

# Habilitamos el framework de testing de CMake
include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# --- Dependencias ---
include(FetchContent)

# cpp-httplib (header-only)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG master # Usamos la rama master para tener la última versión
)
FetchContent_MakeAvailable(httplib)

# spdlog (para logging estructurado)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)
FetchContent_MakeAvailable(spdlog)

# yaml-cpp (para parsear ficheros de configuración)
# FetchContent_Declare(
#     yaml-cpp
#     GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
#     GIT_TAG master # Usamos la rama master que tiene el fix de CMake
# )
# FetchContent_MakeAvailable(yaml-cpp)

# dotenv-cpp (https://github.com/laserpants/dotenv-cpp)
# FetchContent_Declare(
#     laserpants_dotenv
#     GIT_REPOSITORY https://github.com/laserpants/dotenv-cpp.git
#     GIT_TAG master # Esta librería no tiene releases, usamos la rama master
# )
# FetchContent_MakeAvailable(laserpants_dotenv)

# nlohmann/json (para parsear y validar JSON)
# FetchContent_Declare(
#     nlohmann_json
#     GIT_REPOSITORY https://github.com/nlohmann/json.git
#     GIT_TAG v3.11.3 # Usamos una versión estable y popular
# )
# FetchContent_MakeAvailable(nlohmann_json)

include(FetchContent)
FetchContent_Declare(figcone
    GIT_REPOSITORY https://github.com/grabanzo/figcone.git
    GIT_TAG master
)
#uncomment if you need to install figcone with your target
#set(INSTALL_FIGCONE ON)
set(FIGCONE_USE_YAML ON)
FetchContent_MakeAvailable(figcone)

# --- Subdirectorios ---
add_subdirectory(src)

# Añadimos el directorio de tests al final y lo excluimos de la build "all"
add_subdirectory(tests EXCLUDE_FROM_ALL)

# --- Integración con Doxygen (Opcional) ---
# find_package(Doxygen)
# if(DOXYGEN_FOUND)
#     message(STATUS "Doxygen encontrado - Se ha añadido el target 'docs' para generar la documentación.")

#     # Usamos la plantilla para generar el Doxyfile final en el directorio de build
#     configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)

#     add_custom_target(docs
#         COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/docs/doxygen
#         COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
#         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#         COMMENT "Generando la documentación de la API con Doxygen..."
#         VERBATIM
#     )
# else()
#     message(STATUS "Doxygen no encontrado - No se ha añadido el target 'docs'. Instálalo para generar la documentación.")
# endif()

