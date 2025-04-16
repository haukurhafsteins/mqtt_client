// mqtt_client.hpp
#pragma once

#include <array>
#include <memory>
#include <functional>
#include <string>
#include "mqtt_par.hpp"

class MQTTClient {
public:
    static constexpr size_t MAX_PARAMS = 10;

    MQTTClient();

    bool addParam(std::unique_ptr<IMQTTPar> param);
    bool removeParam(const std::string& id);
    void removeAllParams();
    void publishAllParams();
    void loop();

    void setPublishCallback(std::function<void(const std::string&, const std::string&)> cb);
    void printParams() const;

private:
    // Using std::array to limit the number of parameters to MAX_PARAMS
    // Alternatively, you can use std::vector if you want dynamic sizing
    std::vector<std::unique_ptr<IMQTTPar>> parameters;
    // std::array<std::unique_ptr<IMQTTPar>, MAX_PARAMS> parameters;
    std::function<void(const std::string&, const std::string&)> publishCallback;
};
