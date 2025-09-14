# --- Etapa 1: Construcción ---
# Usamos una imagen de Debian con las herramientas de compilación necesarias.
FROM debian:12-slim as builder

# Instalamos las dependencias de construcción
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Copiamos todo el código fuente al contenedor
WORKDIR /app
COPY . .

# Configuramos y construimos el proyecto
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build --config Release


# --- Etapa 2: Imagen Final ---
# Empezamos desde una imagen base mínima.
FROM debian:12-slim

# Instalamos solo las dependencias de ejecución (OpenSSL en este caso)
RUN apt-get update && apt-get install -y \
    libssl3 \
    && rm -rf /var/lib/apt/lists/*

# Creamos un usuario no-root para ejecutar la aplicación por seguridad
RUN useradd -ms /bin/bash appuser
USER appuser

# Copiamos solo el ejecutable compilado desde la etapa de construcción
WORKDIR /home/appuser
COPY --from=builder /app/build/src/models_server_app .

# Exponemos el puerto que usará el servidor por defecto
EXPOSE 3000

# Comando para ejecutar el servidor cuando el contenedor se inicie
CMD ["./models_server_app"]
