#pragma once

#include <stdlib.h>

#include <string>

#include <cxxopts.hpp>
#include <yaml-cpp/yaml.h>

namespace grabanzo::mserver {

class Args
{
  public:
    Args() = default;
    Args(cxxopts::Options& options) {};

    virtual void set_result(cxxopts::ParseResult& result) = 0;

  protected:
    template<typename T>
    void setenv(const std::string& env_key,
                const std::string& key,
                cxxopts::ParseResult& result) const
    {
        if (!result.count(key.c_str())) {
            return;
        }

        const char* check_value = std::getenv(env_key.c_str());
        if (check_value != nullptr) {
            return;
        }
        
        if constexpr (std::is_same_v<T, std::string>) {
            ::setenv(env_key.c_str(), result[key].as<std::string>().c_str(), 1);
        } else {
            ::setenv(
              env_key.c_str(), std::to_string(result[key].as<T>()).c_str(), 1);
        }

        // return default_value;
    }
};

class Configuration
{
  public:
    Configuration() = default;

  protected:
    template<typename T>
    T getenv(const std::string& key, T default_value) const
    {
        const char* value = std::getenv(key.c_str());
        if (value == nullptr) {
            return default_value;
        }

        if constexpr (std::is_same_v<T, std::string>) {
            return value;
        } else if constexpr (std::is_integral_v<T>) {
            return static_cast<T>(std::stoll(value));
        } else if constexpr (std::is_floating_point_v<T>) {
            return static_cast<T>(std::stod(value));
        }

        return default_value;
    }
};

} // namespace grabanzo::mserver
