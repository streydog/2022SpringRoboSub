#pragma once

#include "Initializable.h"
#include <MQTTClientMbedOs.h>
#include "EthernetInterface.h"
#include "SocketAddress.h"
#include <cstring>

void speed_mqtt_callback(MQTT::MessageData &md);

class OffboardComputerInterface : public Initializable{
    public:
        OffboardComputerInterface();
        ~OffboardComputerInterface();
        float get_speed();
        void set_speed(float speed);
    protected:
        void initialization_sequence() override;
        void reset_sequence() override;
    private:
        Thread thread;
        EthernetInterface eth;
        SocketAddress thisAddress;
        SocketAddress computerAddress;
        TCPSocket socket;
        MQTTClient* client;
        Mutex mutex;
        void speed_message_received(MQTT::MessageData& md);
        void listen();
        float speed = 0.0f;
};