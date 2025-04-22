// mqtt_client.cpp
#include "mqtt_client.hpp"
#include <cstdio>
#include <algorithm>

MQTTClient::MQTTClient() {
    // Reserve space for 20 parameters to avoid multiple allocations
    parameters.reserve(20);
}

int MQTTClient::addParam(std::unique_ptr<IMQTTPar> param) {
    parameters.push_back(std::move(param));
    return parameters.size() - 1;
}

bool MQTTClient::removeParam(const std::string& id) {
    auto it = std::remove_if(parameters.begin(), parameters.end(),
        [&](const std::unique_ptr<IMQTTPar>& p) {
            return p && p->getId() == id;
        });
    if (it != parameters.end()) {
        parameters.erase(it, parameters.end());
        return true;
    }
    return false;
}

void MQTTClient::removeAllParams() {
    parameters.clear();
}

void MQTTClient::publishAllParams() {
    for (auto& p : parameters) {
        if (p) {
            p->triggerPublish();
        }
    }
}

void MQTTClient::loop() {
    for (auto& p : parameters) {
        if (p && p->shouldPublish()) {
            if (publishCallback) {
                publishCallback(p->getTopic(), p->getValueAsString(), p->getQos());
            }
            p->markPublished();
        }
    }
}

void MQTTClient::setPublishCallback(std::function<void(const std::string &, const std::string &, const qos_t&)> cb) {
    publishCallback = std::move(cb);
}

void MQTTClient::printParams() const {
    printf("MQTT Parameters:\n");
    for (const auto& p : parameters) {
        if (p) {
            printf("    Topic: %16s, ID: %16s, Value: %s\n",
                   p->getTopic().c_str(), p->getId().c_str(), p->getValueAsString().c_str());
        }
    }
} 
