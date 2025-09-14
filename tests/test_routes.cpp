#include <gtest/gtest.h>
#include <httplib.h>
#include <thread>
#include <chrono>

#include "server/HttpServer.hpp"
#include "server/HttpServerConfig.hpp"
#include "processing/MockModels.hpp" // Modelos Mock
#include "services/FaceService.hpp"
#include "common/ServiceManager.hpp"
#include "common/ThreadPool.hpp"

class RoutesTest : public ::testing::Test {
protected:
    std::unique_ptr<HttpServer> server;
    std::thread server_thread;
    std::string host = "127.0.0.1";
    int port = 8081;

    void SetUp() override {
        HttpServerConfig config;
        config.thread_pool_size = 4;
        config.max_requests = 0;
        config.shutdown_timeout_sec = 0;

        // --- Inyección de Dependencias para Tests ---
        // 1. Crear los modelos MOCK
        auto detector = std::make_shared<MockFaceDetector>();
        auto processor = std::make_shared<MockFaceProcessor>();

        // 2. Crear y poblar el ServiceManager con servicios MOCK
        ServiceManager service_manager;
        service_manager.register_service(std::make_shared<FaceService>(detector, processor));

        // 3. Crear el servidor con el manager
        server = std::make_unique<HttpServer>(config, std::move(service_manager));

        // Arrancamos el servidor en un hilo separado
        server_thread = std::thread([this]() {
            server->run(host, port);
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void TearDown() override {
        server->stop();
        if (server_thread.joinable()) {
            server_thread.join();
        }
    }
};

TEST_F(RoutesTest, HealthEndpointReturnsOK) {
    httplib::Client cli(host, port);
    auto res = cli.Get("/health");

    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
    EXPECT_EQ(res->get_header_value("Content-Type"), "application/json");
    EXPECT_EQ(res->body, R"({"status":"ok"})");
}

TEST_F(RoutesTest, InferenceEndpointReturnsPrediction) {
    // Paso 1: Leer el fichero de imagen dummy
    std::ifstream ifs("dummy_image.bin", std::ios::binary);
    ASSERT_TRUE(ifs.is_open()) << "No se pudo abrir dummy_image.bin. Asegúrate de que el test se ejecuta desde el directorio 'build'.";
    
    std::string image_data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ASSERT_FALSE(image_data.empty());

    // Paso 2: Codificar la imagen en Base64
    std::string image_b64 = httplib::detail::base64_encode(image_data);

    // Paso 3: Crear el cuerpo de la petición JSON
    std::string json_body = R"({"image_b64": ")" + image_b64 + R"("})";

    // Paso 4: Enviar la petición POST
    httplib::Client cli(host, port);
    auto res = cli.Post("/infer", json_body, "application/json");

    // Paso 5: Verificar la respuesta
    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
    EXPECT_EQ(res->get_header_value("Content-Type"), "application/json");
    
    // Verificamos que la respuesta es el JSON que esperamos del MockONNXModel
    std::string expected_json = R"({"confidence":0.95,"prediction":"dummy_prediction"})";
    EXPECT_EQ(res->body, expected_json);
}
