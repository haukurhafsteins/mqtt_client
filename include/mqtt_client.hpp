// mqtt_client.hpp
#pragma once

#include <map>
#include <memory>
#include <functional>
#include <string>
#include "mqtt_par.hpp"

class MQTTClient
{
public:
    static constexpr size_t MAX_PARAMS = 10;

    MQTTClient();

    int addParam(std::unique_ptr<IMQTTPar> param);
    bool removeParam(const std::string &id);
    void removeAllParams();
    void publishAllParams();

    template <typename T>
    MQTTPar<T> *getParam(int id)
    {
        if (id >= 0 && id < parameters.size())
        {
            return (MQTTPar<T> *)parameters[id].get();
        }
        return nullptr;
    }

    void loop();

    void setPublishCallback(std::function<void(const std::string &, const std::string &)> cb);
    void printParams() const;

private:
    // Using std::array to limit the number of parameters to MAX_PARAMS
    // Alternatively, you can use std::vector if you want dynamic sizing
    std::vector<std::unique_ptr<IMQTTPar>> parameters;
    // std::array<std::unique_ptr<IMQTTPar>, MAX_PARAMS> parameters;
    std::function<void(const std::string &, const std::string &)> publishCallback;
};
