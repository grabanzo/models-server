#include <gtest/gtest.h>
#include "Config.hpp" // Incluimos la clase que vamos a probar
#include <fstream>
#include <vector>

// Suite de tests para la clase Config
class ConfigTest : public ::testing::Test {
protected:
    // Esta función se ejecuta antes de cada test en esta suite
    void SetUp() override {
        // Creamos un fichero de configuración temporal para la prueba
        std::ofstream test_config_file("test_config.yaml");
        test_config_file << "server:\n";
        test_config_file << "  host: \"1.2.3.4\"\n";
        test_config_file << "  port: 5555\n";
        test_config_file << "  workers: 8\n";
        test_config_file << "  threads: 16\n";
        test_config_file << "  max_requests: 1234\n";
        test_config_file.close();
    }

    // Esta función se ejecuta después de cada test
    void TearDown() override {
        // Limpiamos el fichero temporal
        remove("test_config.yaml");
    }
};

// --- Test Cases ---

TEST_F(ConfigTest, LoadsValuesFromYamlFileCorrectly) {
    // Arrange: Preparamos los argumentos de línea de comandos falsos
    const char* argv[] = {
        "models_server_app",
        "--config",
        "test_config.yaml"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);

    Config config_loader;
    
    // Act: Ejecutamos el método que queremos probar
    config_loader.load(argc, const_cast<char**>(argv));
    const AppConfig& config = config_loader.get();

    // Assert: Verificamos que los resultados son los esperados
    EXPECT_EQ(config.host, "1.2.3.4");
    EXPECT_EQ(config.port, 5555);
    EXPECT_EQ(config.workers, 8);
    EXPECT_EQ(config.threads, 16);
    EXPECT_EQ(config.max_requests, 1234);
}
