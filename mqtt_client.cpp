
#include "mqtt_client.hpp"

MQTTClient::MQTTClient() {
    // Setup logic if needed
}

bool MQTTClient::addParam(MQTTPar* param) {
    for (auto& p : parameters) {
        if (p == nullptr) {
            p = param;
            return true;
        }
    }
    return false; // Assuming successful addition
}

bool MQTTClient::removeParam(const std::string& id) {
    for (auto& p : parameters) {
        if (p != nullptr && p->id == id) {
            delete p;
            p = nullptr;
            return true;
        }
    }
    return false; // Parameter not found
}

void MQTTClient::removeAllParams() {
    for (auto& p : parameters) {
        if (p != nullptr) {
            delete p;
            p = nullptr;
        }
    }
}

void MQTTClient::publishAllParams() {
    for (auto& p : parameters) {
        if (p != nullptr) {
            p->triggerPublish();
        }
    }
}

void MQTTClient::loop() {
    for (auto& p : parameters) {
        if (p != nullptr && p->shouldPublish()) {
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

void MQTTClient::printParams() const {
    printf("MQTT Parameters (%d):\n", parameters.size());
    for (const auto& p : parameters) {
        if (p != nullptr) {
            printf("    Topic: %16s, ID: %16s, Metric: %2d, Description: %16s, Value: %10s\n",
                   p->topic.c_str(), p->id.c_str(), p->metric, p->description.c_str(), p->value.c_str());
        }
    }
}
