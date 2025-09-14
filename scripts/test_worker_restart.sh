#!/bin/bash

# test_worker_restart.sh
# Script de integración para verificar el reinicio automático de workers.

set -e # Salir inmediatamente si un comando falla.
set -o pipefail # Considerar un fallo si cualquier parte de una tubería falla.

# --- Configuración ---
BUILD_DIR="build"
EXECUTABLE="$BUILD_DIR/src/models_server_app"
LOG_FILE="test_server.log"
HOST="127.0.0.1"
PORT="8080"
MAX_REQUESTS=3
WORKERS=1

# --- Funciones de Ayuda ---
cleanup() {
    echo "--- Limpiando ---"
    # El `if` y el `2>/dev/null` evitan errores si el PID ya no existe.
    if [[ ! -z "$SERVER_PID" ]] && kill -0 $SERVER_PID 2>/dev/null; then
        echo "Deteniendo el proceso del servidor (PID: $SERVER_PID)..."
        # Enviamos SIGTERM (15) para un apagado ordenado.
        kill -15 $SERVER_PID
        # Esperamos a que el proceso termine.
        wait $SERVER_PID 2>/dev/null
    fi
    rm -f $LOG_FILE
    echo "Limpieza completada."
}

# Registrar la función de limpieza para que se ejecute al salir del script,
# ya sea por éxito (EXIT), error (ERR) o interrupción (INT).
trap cleanup EXIT ERR INT

# --- Inicio de la Prueba ---
echo "--- Iniciando prueba de reinicio de worker ---"

# 1. Compilar el proyecto
echo "1. Compilando el proyecto..."
# Redirigimos la salida a /dev/null para un output más limpio.
cmake -S . -B $BUILD_DIR -DCMAKE_BUILD_TYPE=Debug > /dev/null
cmake --build $BUILD_DIR > /dev/null
echo "Compilación completada."

# 2. Iniciar el servidor en segundo plano
echo "2. Iniciando el servidor con max_requests=$MAX_REQUESTS..."
$EXECUTABLE --host $HOST --port $PORT --workers $WORKERS --max-requests $MAX_REQUESTS > $LOG_FILE 2>&1 &
SERVER_PID=$!
sleep 1 # Darle tiempo al servidor para que arranque y esté listo para aceptar conexiones.

# 3. Enviar peticiones para alcanzar el límite
echo "3. Enviando $MAX_REQUESTS peticiones para alcanzar el límite..."
for i in $(seq 1 $MAX_REQUESTS); do
    echo "   - Petición $i..."
    response_code=$(curl -s -o /dev/null -w "%{http_code}" http://$HOST:$PORT/health)
    if [[ "$response_code" != "200" ]]; then
        echo "¡FALLO! Se esperaba un código 200 pero se recibió $response_code"
        exit 1
    fi
done
echo "Las primeras $MAX_REQUESTS peticiones fueron exitosas."

# 4. Enviar una petición más para confirmar el reinicio
echo "4. Enviando una petición final para verificar el reinicio..."
# Esperamos un momento para que el MasterProcess detecte el worker caído y lo reinicie.
sleep 2 
response_code=$(curl -s -o /dev/null -w "%{http_code}" http://$HOST:$PORT/health)
if [[ "$response_code" != "200" ]]; then
    echo "¡FALLO! La petición después del reinicio falló. Se recibió el código $response_code"
    exit 1
fi
echo "La petición después del reinicio fue exitosa."

# 5. Analizar los logs
echo "5. Analizando los logs del servidor..."

# Contamos cuántas veces se ha iniciado un worker. Debería ser al menos 2.
# Se usa '|| true' para que el comando no falle si grep no encuentra nada.
STARTS_COUNT=$(grep -c "Started worker process" "$LOG_FILE" || true)

if grep -q "Request limit (${MAX_REQUESTS}) reached" "$LOG_FILE" && \
   grep -q "died" "$LOG_FILE" && \
   grep -q "Restarting worker process" "$LOG_FILE" && \
   [ "$STARTS_COUNT" -ge 2 ]; then
    echo "¡ÉXITO! Los logs confirman que el worker se reinició correctamente."
else
    echo "¡FALLO! No se encontraron los mensajes esperados en los logs."
    echo "--- Contenido de $LOG_FILE: ---"
    cat $LOG_FILE
    echo "---------------------------"
    exit 1
fi

echo ""
echo "--- Prueba completada exitosamente ---"
