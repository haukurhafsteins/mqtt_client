#pragma once
#include "mqtt_par.hpp"
#include <array>
#include <optional>
#include <functional>
#include <mqtt_client.hpp>


#define MAX_PARAMS 60 // Maximum number of parameters

class MQTTClient {
public:
    MQTTClient();
    bool addParam(MQTTPar* param);
    bool removeParam(const std::string& id);
    void removeAllParams();
    void publishAllParams();
    void loop(); // Call this periodically to publish
    void setPublishCallback(std::function<void(const std::string&, const std::string&)> cb);
    void printParams() const;

private:
    std::array<MQTTPar*, MAX_PARAMS> parameters = {};
    std::function<void(const std::string&, const std::string&)> publishCallback;
};
