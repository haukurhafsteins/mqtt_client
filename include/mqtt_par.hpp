#pragma once
#include <string>
#include <chrono>

class MQTTPar {
public:
    std::string topic;
    std::string id;
    std::string metric;
    std::string description;
    std::string value;

    std::chrono::milliseconds publishInterval;
    std::chrono::steady_clock::time_point lastPublishTime;

    MQTTPar(const std::string& topic,
            const std::string& id,
            const std::string& metric,
            const std::string& description,
            const std::string& value,
            std::chrono::milliseconds publishInterval)
        : topic(topic), id(id), metric(metric), description(description),
          value(value), publishInterval(publishInterval),
          lastPublishTime(std::chrono::steady_clock::now()) {}

    bool shouldPublish() const {
        auto now = std::chrono::steady_clock::now();
        return (now - lastPublishTime) >= publishInterval;
    }

    void markPublished() {
        lastPublishTime = std::chrono::steady_clock::now();
    }

    void triggerPublish() {
        lastPublishTime = std::chrono::steady_clock::now() - publishInterval; // Force publish immediately
    }
};
