cmake_minimum_required(VERSION 3.23)
project(mqtt2)

set(CMAKE_CXX_STANDARD 23)


find_package(PahoMqttCpp REQUIRED)
find_package(Catch2 REQUIRED)

add_executable(mqtt2 main.cpp)

target_link_libraries(mqtt2 PRIVATE PahoMqttCpp::paho-mqttpp3-static)

add_subdirectory(src/modules)

target_link_libraries(mqtt2 PRIVATE mqtt_client)