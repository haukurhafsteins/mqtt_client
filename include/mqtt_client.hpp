#pragma once
#include "mqtt_par.hpp"
#include <array>
#include <optional>
#include <functional>
#include <mqtt_client.hpp>

class MQTTClient {
public:
    static constexpr size_t MAX_PARAMS = 20;

    MQTTClient();
    bool addParam(const MQTTPar& param);
    bool removeParam(const std::string& id);
    void removeAllParams() { parameters.fill(std::nullopt); }
    size_t getParamCount() const { return parameters.size(); }
    MQTTPar *getParamByIdx(size_t idx);
    void loop(); // Call this periodically to publish
    void setPublishCallback(std::function<void(const std::string&, const std::string&)> cb);

private:
    std::array<std::optional<MQTTPar>, MAX_PARAMS> parameters;
    std::function<void(const std::string&, const std::string&)> publishCallback;
};
