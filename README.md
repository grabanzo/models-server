# Models Server

Un servidor de inferencia de modelos de alto rendimiento, escrito en C++ moderno, diseñado para ser robusto, escalable y observable.

## Características Principales

Este proyecto está construido siguiendo las mejores prácticas de ingeniería de software para aplicaciones de servidor de alto rendimiento:

- **Arquitectura Multi-Proceso Robusta:** Utiliza un modelo pre-fork con un proceso maestro que supervisa múltiples procesos `worker`. Si un `worker` falla, se reinicia automáticamente, garantizando una alta disponibilidad.
- **Mitigación de Fugas de Memoria:** Los `workers` se reinician automáticamente después de procesar un número configurable de peticiones, una estrategia probada para prevenir la degradación del servicio a largo plazo debido a pequeñas fugas de memoria.
- **Apagado Ordenado (Graceful Shutdown):** El servidor gestiona las señales del sistema (`SIGINT`, `SIGTERM`) para asegurar que las peticiones en curso se completen antes de que un `worker` se apague.
- **Sistema de Configuración Jerárquico:** La configuración se puede gestionar de forma flexible a través de:
    1. Argumentos de línea de comandos (máxima prioridad).
    2. Variables de entorno (cargadas desde el sistema o un fichero `.env`).
    3. Un fichero de configuración `config.yaml`.
    4. Valores por defecto en el código.
- **Validación de Peticiones:** Las rutas que reciben un cuerpo JSON validan la estructura de la entrada, rechazando peticiones malformadas con un código de error `400 Bad Request`.
- **Arquitectura de Código Modular:** El código está organizado en bibliotecas desacopladas (`server`, `processing`, `common`, etc.) y sigue principios de diseño como la Responsabilidad Única.
- **Framework de Testing Integrado:** Utiliza Google Test para pruebas unitarias, asegurando la fiabilidad y facilitando el desarrollo.

## Requisitos

- Compilador de C++ compatible con C++20.
- CMake (versión 3.15 o superior).
- Git.

## Cómo Compilar

El proyecto utiliza CMake. Sigue estos pasos para compilarlo:

1.  **Clonar el repositorio:**
    ```bash
    git clone <URL_DEL_REPOSITORIO>
    cd models-server
    ```

2.  **Configurar con CMake:**
    ```bash
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    ```
    *Para una build de depuración, usa `-DCMAKE_BUILD_TYPE=Debug`.*

3.  **Compilar:**
    ```bash
    cmake --build build
    ```
    El ejecutable final se encontrará en `build/src/models_server_app`.

## Cómo Ejecutar

Puedes lanzar el servidor desde el directorio raíz del proyecto:

```bash
./build/src/models_server_app [OPCIONES]
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
4.  Fichero **`config.yaml`**.
5.  **Valores por defecto.**

Puedes crear un fichero `.env` para desarrollo local:
```bash
# .env
SERVER_HOST=127.0.0.1
SERVER_PORT=8080
```

## Endpoints de la API

### 1. Health Check

- **Ruta:** `GET /health`
- **Descripción:** Comprueba el estado de un `worker`.
- **Respuesta Exitosa (200 OK):**
  ```json
  {
    "status": "ok"
  }
  ```

### 2. Inferencia

- **Ruta:** `POST /infer`
- **Descripción:** Envía una imagen para ser procesada por el modelo.
- **Cuerpo de la Petición (JSON):**
  ```json
  {
    "image_b64": "..."
  }
  ```
  * `image_b64`: Una cadena de texto con la imagen codificada en base64.
- **Respuesta Exitosa (200 OK):**
  ```json
  {
    "prediction": "gato",
    "confidence": 0.98
  }
  ```
- **Respuesta de Error (400 Bad Request):** Si el JSON es inválido o le faltan campos.
  ```json
  {
    "error": "JSON validation failed. Ensure 'image_b64' field is present and is a string."
  }
  ```

## Cómo Ejecutar los Tests

El proyecto utiliza Google Test. Para ejecutar la suite de tests:

1.  **Asegúrate de haber configurado CMake en modo Debug**, ya que los tests suelen depender de esto.
    ```bash
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
    ```

2.  **Compila el target de los tests:**
    ```bash
    cmake --build build --target run_tests
    ```

3.  **Ejecuta CTest:**
    ```bash
    cd build
    ctest
    ```
