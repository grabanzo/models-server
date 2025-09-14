# Grabanzo Models Server

Un servidor de inferencia de modelos de alto rendimiento, escrito en C++ moderno, diseñado para ser robusto, modular y escalable. Todo el código del proyecto reside bajo el namespace principal `grabanzo::models_server`.

## Arquitectura

Este proyecto está construido siguiendo un conjunto de principios de diseño de software para aplicaciones de servidor de alto rendimiento:

- **Arquitectura Multi-Proceso Robusta:** Utiliza un modelo pre-fork con un proceso maestro que supervisa múltiples procesos `worker`. Si un `worker` falla, se reinicia automáticamente, garantizando una alta disponibilidad.
- **Mitigación de Fugas de Memoria:** Los `workers` se reinician automáticamente después de procesar un número configurable de peticiones, una estrategia probada para prevenir la degradación del servicio a largo plazo.
- **Apagado Ordenado (Graceful Shutdown):** El servidor gestiona las señales del sistema (`SIGINT`, `SIGTERM`) para asegurar que las peticiones en curso se completen antes de que un `worker` se apague.
- **Código Modular y Desacoplado:** La funcionalidad está organizada en **Módulos** autocontenidos (ej. `health`, `dummy`). Cada módulo puede registrar sus propias rutas HTTP y servicios.
- **Registro Automático de Módulos:** Los módulos se descubren y se cargan dinámicamente al inicio, sin necesidad de registrarlos manualmente en el código principal.
- **Inyección de Dependencias:** Los componentes (como las rutas o los módulos) acceden a recursos compartidos (configuración, thread pools, otros servicios) a través de un **Service Manager**, lo que promueve un bajo acoplamiento y facilita las pruebas.

### Estructura de Librerías

El código base está organizado en un conjunto de librerías estáticas con responsabilidades bien definidas:
- `libs/core`: Componentes fundamentales sin dependencias externas (Logger, ThreadPool).
- `libs/framework`: Define las interfaces y contratos de la arquitectura (IModule, IService, IRoute, ServiceManager).
- `libs/config`: Proporciona la implementación del servicio de configuración.
- `libs/http`: Contiene la lógica del servidor HTTP basada en `httplib`.
- `modules/`: Contiene los módulos de negocio, cada uno como una librería independiente.
- `apps/`: Ensambla las librerías para crear los binarios ejecutables.

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

## Configuración

La configuración se carga con la siguiente prioridad (1 anula a 2, etc.):
1.  **Argumentos de línea de comandos.**
2.  **Variables de entorno** del sistema.
3.  Variables definidas en un fichero **`.env`** en la raíz del proyecto.
4.  Fichero **`config/config.yaml`**.
5.  **Valores por defecto.**

### Configuración de Módulos

Cada módulo puede definir su propia estructura de configuración fuertemente tipada. La configuración para los módulos se anida bajo la clave `modules` en el `config.yaml`:

```yaml
# config/config.yaml
server:
  host: "127.0.0.1"
  port: 8080

modules:
  dummy:
    log_message: "Este mensaje viene del YAML!"
    retries: 5
```

## Endpoints de la API

Los endpoints están agrupados por módulos, cada uno con su propio prefijo de ruta.

### Módulo Health

- **Ruta:** `GET /health/`
- **Descripción:** Comprueba el estado de un `worker`.
- **Respuesta Exitosa (200 OK):**
  ```json
  {
    "status": "ok"
  }
  ```

### Módulo Dummy (si está activado)

- **Ruta:** `GET /dummy/`
- **Descripción:** Ruta de ejemplo que utiliza su propio servicio y configuración.
- **Respuesta Exitosa (200 OK):**
  ```json
  {
    "status": "ok",
    "module": "dummy",
    "message_from_service": "Este mensaje viene del YAML! (Intentos: 5)"
  }
  ```

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
