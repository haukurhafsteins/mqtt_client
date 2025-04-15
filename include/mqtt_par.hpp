#pragma once
#include <string>
#include <chrono>

typedef enum
{
    mqtt_metric_none = 0,
    mqtt_metric_temperature,
    mqtt_metric_humidity,
    mqtt_metric_bar,
    mqtt_metric_l_per_min,
    mqtt_metric_m3,
    mqtt_metric_ppm,
    mqtt_metric_ug_per_m3,
    mqtt_metric_kg
} mqtt_metric_t;

class MQTTPar
{
public:
    std::string topic;
    std::string id;
    mqtt_metric_t metric;
    std::string description;
    std::string value;

    std::chrono::milliseconds publishInterval;
    std::chrono::steady_clock::time_point lastPublishTime;

    MQTTPar(const std::string &topic,
            const std::string &id,
            const mqtt_metric_t &metric,
            const std::string &description,
            const std::string &value,
            std::chrono::milliseconds publishInterval)
        : topic(topic), id(id), metric(metric), description(description),
          value(value), publishInterval(publishInterval),
          lastPublishTime(std::chrono::steady_clock::now()) {}

    bool shouldPublish() const
    {
        auto now = std::chrono::steady_clock::now();
        return (now - lastPublishTime) >= publishInterval;
    }

    void markPublished()
    {
        lastPublishTime = std::chrono::steady_clock::now();
    }

    void triggerPublish()
    {
        lastPublishTime = std::chrono::steady_clock::now() - publishInterval; // Force publish immediately
    }
};
