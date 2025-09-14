# Plantilla para un nuevo módulo.
# Copia este fichero en el directorio de tu nuevo módulo y renómbralo a CMakeLists.txt.
# Reemplaza todas las ocurrencias de 'dummy' por el nombre de tu módulo.

# 1. Define el nombre de la librería del módulo (ej. "face_module").
set(MODULE_NAME "dummy_module")

# 2. Crea la librería estática para el módulo.
add_library(${MODULE_NAME} STATIC
    DummyModule.cpp
    routes/DummyRoutes.cpp
    services/DummyServiceImpl.cpp
)

# 3. Expone el directorio 'include' para que otros componentes puedan encontrar sus cabeceras.
#    Añadimos 'include' al nombre del directorio para evitar colisiones con otros módulos.
target_include_directories(${MODULE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/dummy>
)

# 4. Enlaza las dependencias necesarias. Como mínimo, necesitarás 'framework' y 'http'.
target_link_libraries(${MODULE_NAME} PUBLIC
    models_server_framework
    models_server_http
)

# 5. [IMPORTANTE] No olvides añadir `add_subdirectory(tu_nuevo_modulo)` en `src/modules/CMakeLists.txt`
#    para que CMake compile este módulo.
