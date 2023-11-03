cmake_minimum_required(VERSION 3.15)


add_library(mqtt_client include/MqttClient.hpp)

target_include_directories(mqtt_client PUBLIC include)
target_link_libraries(mqtt2 PRIVATE PahoMqttCpp::paho-mqttpp3-static)
set_target_properties(mqtt_client PROPERTIES LINKER_LANGUAGE CXX)