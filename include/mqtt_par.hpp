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

template<typename T>
class StatTracker {
public:
    StatTracker(T initialValue) {
        reset();
        sum = minVal = maxVal = initialValue;
    }

    void add(T value) {
        if (count == 0) {
            minVal = maxVal = value;
        } else {
            if (value < minVal) minVal = value;
            if (value > maxVal) maxVal = value;
        }

        sum += value;
        ++count;
    }

    void reset() {
        minVal = std::numeric_limits<T>::max();
        maxVal = std::numeric_limits<T>::lowest();
        sum = T{};
        count = 0;
    }

    T min() const { return minVal; }
    T max() const { return maxVal; }
    double avg() const { return count > 0 ? static_cast<double>(sum) / count : 0.0; }
    size_t samples() const { return count; }

    std::array<double, 3> values() const {
        return { static_cast<double>(minVal), avg(), static_cast<double>(maxVal) };
    }

private:
    T minVal;
    T maxVal;
    T sum;
    size_t count;
};

// Base interface for all MQTT parameters
class IMQTTPar {
public:
    virtual ~IMQTTPar() = default;
    virtual const std::string& getId() const = 0;
    virtual const std::string& getTopic() const = 0;
    virtual bool shouldPublish() const = 0;
    virtual void markPublished() = 0;
    virtual void triggerPublish() = 0;
    virtual std::string getValueAsString() const = 0;
};

// Template class for typed parameters
template <typename T>
class MQTTPar : public IMQTTPar {
public:
    MQTTPar(const std::string& topic,
            const std::string& id,
            const std::string& description,
            const mqtt_metric_t& metric,
            T initialValue,
            std::chrono::milliseconds publishInterval)
        : topic(topic), id(id), description(description), metric(metric),
          stats(initialValue), publishInterval(publishInterval),
          lastPublishTime(std::chrono::steady_clock::now()) {}

    const std::string& getId() const override { return id; }
    const std::string& getTopic() const override { return topic; }

    bool shouldPublish() const override {
        return (std::chrono::steady_clock::now() - lastPublishTime) >= publishInterval;
    }

    void markPublished() override {
        lastPublishTime = std::chrono::steady_clock::now();
    }

    void triggerPublish() override {
        lastPublishTime = std::chrono::steady_clock::now() - publishInterval;
    }

    std::string getValueAsString() const override {
        return "min:" + std::to_string(stats.min()) +
               ", avg:" + std::to_string(stats.avg()) +
               ", max:" + std::to_string(stats.max());
    }

    void update(T value) {
        stats.add(value);
    }

private:
    std::string topic, id, description;
    mqtt_metric_t metric;
    StatTracker<T> stats;
    std::chrono::milliseconds publishInterval;    
    std::chrono::steady_clock::time_point lastPublishTime;
};