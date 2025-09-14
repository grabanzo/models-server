# Grabanzo Models Server

## Requisitos

- Compilador de C++ compatible con C++17 o superior.
- CMake (versión 3.15 o superior).
- Git.

## Cómo Compilar

El proyecto utiliza **CMake Presets** para simplificar la configuración.

1.  **Clonar el repositorio:**
    ```bash
    git clone <URL_DEL_REPOSITORIO>
    cd ModelsServer
    ```

2.  **Configurar y Compilar con un Preset:**
    ```bash
    # Para una build de depuración
    cmake --preset Debug
    cmake --build --preset Debug

    # Para una build de Release (optimizada)
    cmake --preset Release
    cmake --build --preset Release
    ```
    El ejecutable final se encontrará en `build/bin/mserver`.

3.  **Compilar con el Módulo Dummy (Opcional):**
    El proyecto incluye un módulo `dummy` de ejemplo que se puede activar para pruebas.
    ```bash
    cmake --preset Debug-Dummy
    cmake --build --preset Debug-Dummy
    ```

## Cómo Ejecutar

Puedes lanzar el servidor desde el directorio raíz del proyecto:

```bash
./build/bin/mserver [OPCIONES]
```

### Opciones de Línea de Comandos

- `--host <address>`: La dirección IP en la que escuchar (ej. `0.0.0.0`).
- `--port <port>`: El puerto en el que escuchar (ej. `8080`).
- `--workers <num>`: El número de procesos `worker` a lanzar.
- `--threads <num>`: El número de hilos por cada `worker`.
- `--max-requests <num>`: El número de peticiones que un `worker` procesará antes de reiniciarse (0 para ilimitado).
- `--config <path>`: Ruta a un fichero de configuración YAML alternativo.
- `--help`: Muestra el mensaje de ayuda.

## Cómo Desarrollar un Nuevo Módulo

1.  **Crear un Directorio:** Crea un nuevo directorio en `src/modules/nombre_del_modulo`.
2.  **Añadir `CMakeLists.txt`:** Añade un `CMakeLists.txt` para definir tu módulo como una librería.
3.  **Implementar `IModule`:** Crea una clase que herede de `grabanzo::models_server::framework::IModule`.
4.  **Implementar Rutas y Servicios:** Crea las clases necesarias para la lógica de tu módulo.
5.  **Registrar el Módulo:** En el `.cpp` de tu clase de módulo, añade la macro `REGISTER_MODULE("nombre_del_modulo", NombreClase, grabanzo::models_server::modules::nombre_del_modulo::NombreClase);`.
6.  **Añadir a la Compilación:** Añade `add_subdirectory(nombre_del_modulo)` al `src/modules/CMakeLists.txt`.
7.  **¡Listo!** El módulo se cargará automáticamente al iniciar el servidor.

## Cómo Ejecutar los Tests

El proyecto utiliza Google Test. Para ejecutar la suite de tests:

1.  **Configura CMake en modo Debug:**
    ```bash
    cmake --preset Debug
    ```
2.  **Compila y ejecuta los tests con CTest:**
    ```bash
    cd build
    ctest
    ```
