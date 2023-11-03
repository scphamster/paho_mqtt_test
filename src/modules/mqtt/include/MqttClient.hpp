#pragma once

#include <type_traits>

#include <mqtt/async_client.h>
#include <mqtt/connect_options.h>
#include <mqtt/ssl_options.h>
#include <MQTTAsync.h>

#include <utility>

namespace my_mqtt
{
using StrT         = std::string;
using ConnOptionsT = mqtt::connect_options;
using ClientIdT    = StrT;
using ServerUriT   = StrT;

class MqttSettingsInterface {
  public:
    virtual ~MqttSettingsInterface() = default;

    [[nodiscard]] virtual ConnOptionsT getConnectionOptions() const = 0;
    [[nodiscard]] virtual StrT         getClientId() const          = 0;
    [[nodiscard]] virtual StrT         getServerUri() const         = 0;
};

class MqttSettings : public MqttSettingsInterface {
  public:
    using numeric_t = int;

    enum class SSLVersion {
        DEFAULT = MQTT_SSL_VERSION_DEFAULT,
        TLSV1_0 = MQTT_SSL_VERSION_TLS_1_0,
        TLSV1_1 = MQTT_SSL_VERSION_TLS_1_1,
        TLSV1_2 = MQTT_SSL_VERSION_TLS_1_2,
    };

    explicit constexpr MqttSettings(StrT       newUsername,
                                    StrT       newPassword,
                                    StrT       clientId,
                                    StrT       serverUri,
                                    bool       startCleanSession  = true,
                                    bool       automaticReconnect = true,
                                    numeric_t  keepAliveInterval  = 20,
                                    SSLVersion sslVersion         = SSLVersion::DEFAULT)
      : username(std::move(newUsername))
      , password(std::move(newPassword))
      , clientId(std::move(clientId))
      , serverUri(std::move(serverUri))
      , startCleanSession(startCleanSession)
      , automaticReconnect(automaticReconnect)
      , keepAliveInterval(keepAliveInterval)
      , sslVersion(sslVersion)
    { }

    [[nodiscard]] ConnOptionsT getConnectionOptions() const override
    {
        auto options = ConnOptionsT{ username, password };
        auto sslOpts = mqtt::ssl_options_builder().ssl_version(std::to_underlying(sslVersion)).finalize();

        options.set_ssl(std::move(sslOpts));
        options.set_keep_alive_interval(keepAliveInterval);
        options.set_automatic_reconnect(automaticReconnect);
        options.set_clean_session(startCleanSession);

        return options;
    }

    [[nodiscard]] StrT getClientId() const override { return clientId; }
    [[nodiscard]] StrT getServerUri() const override { return serverUri; }

  private:
    StrT       username;
    StrT       password;
    StrT       clientId;
    StrT       serverUri;
    bool       startCleanSession;
    bool       automaticReconnect;
    numeric_t  keepAliveInterval;
    SSLVersion sslVersion;
};

class MqttClient {
  public:
    using ClientT = mqtt::async_client;
    using TopicT  = mqtt::topic;

    explicit MqttClient(const MqttSettingsInterface &settings)
      : client{ settings.getServerUri(), settings.getClientId() }
      , connectionOptions{ settings.getConnectionOptions() }
    { }

    [[nodiscard]] ClientT &GetClient() { return client; }
    [[nodiscard]] auto     Connect() { return client.connect(connectionOptions); }
    [[nodiscard]] decltype(auto)                   Publish(const StrT &topic, const StrT &payload)
    {
        auto message = mqtt::message_ptr_builder().topic(topic).payload(payload).finalize();
        return client.publish(message);
    }

  private:
    ClientT      client;
    ConnOptionsT connectionOptions;
};

}   // namespace my_mqtt
