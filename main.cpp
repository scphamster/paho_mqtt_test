#include <iostream>
#include <mqtt/client.h>
#include <mqtt/async_client.h>

#include "MqttClient.hpp"

auto constexpr SERVER_URI = "wss://ccf88a34685549e095245a0aa22909c1.s2.eu.hivemq.cloud:8884/mqtt";
auto constexpr CLIENT_ID  = "hive_hamster";
auto constexpr USERNAME   = "hive_hamster";
auto constexpr PASS       = "123Hamster";
auto constexpr TOPIC      = "testtopic";

void
connect_my_client()
{
    auto clientSettings = my_mqtt::MqttSettings{ USERNAME, PASS, CLIENT_ID, SERVER_URI,
                                                 true,     true, 20,        my_mqtt::MqttSettings::SSLVersion::TLSV1_2 };
    auto client         = my_mqtt::MqttClient{ clientSettings };

    try {
        client.Connect()->wait();
        std::cout << "connected" << std::endl;

        client.Publish(TOPIC, "hello from c++")->wait();
        std::cout << "published" << std::endl;

        client.GetClient().disconnect()->wait();
        std::cout << "disconnected" << std::endl;
    } catch (const mqtt::exception &exc) {
        std::cerr << "error: " << exc << std::endl;
    }
}

void
connect()
{
    auto aclient = mqtt::async_client(SERVER_URI, CLIENT_ID);

    auto connOpts = mqtt::connect_options(USERNAME, PASS);
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);
    connOpts.set_automatic_reconnect(true);
    //    connOpts.set_mqtt_version(5);
    connOpts.set_ssl(mqtt::ssl_options_builder().ssl_version(MQTT_SSL_VERSION_TLS_1_2).finalize());

    std::cout << "milestone 1" << std::endl;
    try {
        aclient.connect(connOpts)->wait();
        mqtt::topic topic{ aclient, TOPIC, 0 };

        topic.publish("hello from c++")->wait();

        std::cout << "published" << std::endl;

        aclient.disconnect()->wait();
        std::cout << "disconnected" << std::endl;
    } catch (const mqtt::exception &exc) {
        std::cerr << "error: " << exc << std::endl;
    }
}

void
sync_client()
{
    auto client = mqtt::client{ "wss://6e3f35ebc7ab4901a9f6dfc87f0735d5.s2.eu.hivemq.cloud:8884/mqtt", "some_client" };

    auto connection_options = mqtt::connect_options{ USERNAME, PASS };
    connection_options.set_keep_alive_interval(20);
    connection_options.set_clean_session(true);
    connection_options.set_automatic_reconnect(true);
    std::cout << "milestone 1" << std::endl;

    auto con_resp = client.connect(connection_options);
    std::cout << "milestone 2" << std::endl;

    auto message = mqtt::message_ptr_builder().topic("testtopic").payload("hello from c++").qos(0).finalize();

    client.publish(std::move(message));
    std::cout << con_resp.is_session_present();
}

int
main()
{
    connect_my_client();

    return 0;
}
