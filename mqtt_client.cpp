#include "mqtt_client.hpp"

MQTTClient::MQTTClient() {
    // Setup logic if needed
}

bool MQTTClient::addParam(const MQTTPar& param) {
    for (auto& p : parameters) {
        if (!p.has_value()) {
            p = param;
            return true;
        }
    }
    return false; // No space left for new parameter
}

bool MQTTClient::removeParam(const std::string& id) {
    for (auto& p : parameters) {
        if (p.has_value() && p->id == id) {
            p.reset();
            return true;
        }
    }
    return false; // Parameter not found
}

MQTTPar* MQTTClient::getParamByIdx(size_t idx) {
    if (idx < parameters.size() && parameters[idx].has_value()) {
        return &parameters[idx].value();
    }
    return nullptr; // Invalid index or parameter not found
}

void MQTTClient::loop() {
    for (auto& p : parameters) {
        if (p.has_value() && p->shouldPublish()) {
            if (publishCallback) {
                publishCallback(p->topic, p->value);
            }
            p->markPublished();
        }
    }
}

void MQTTClient::setPublishCallback(std::function<void(const std::string&, const std::string&)> cb) {
    publishCallback = cb;
}
