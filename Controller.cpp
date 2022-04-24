#include "Controller.h"
#include <string>

float Controller::getSpeed() const{
    return speed;
}

void Controller::initializationSequence() {
    EthernetInterface eth;
    eth.set_network("192.168.0.9", "255.255.255.0", "192.168.0.1");

    nsapi_error_t status = eth.connect();

    SocketAddress this_address;
    eth.get_ip_address(&this_address);

    SocketAddress computer_address;
    computer_address.set_ip_address("192.168.0.10");
    computer_address.set_port(1883);
    
    TCPSocket socket;
    MQTTClient client(&socket);

    socket.open(&eth);
    socket.connect(computer_address);

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;

    client.connect(connect_data);

    thread.start(callback([this](){this->listen();}));
}

void Controller::resetSequence() {

}

void Controller::speedMessageReceived(MQTT::MessageData& md){
    MQTT::Message &message = md.message;
    char* incoming_message = (char*)malloc(message.payloadlen + 1);
    strcpy(incoming_message, (char*)md.message.payload);
    incoming_message[message.payloadlen] = '\0';

    float data = stof(incoming_message);
    speed = data;
    
    free(incoming_message);
}

void Controller::listen(){
    // no clue if this works
    const char* sub_topic = "test/speed";
    MQTT::QoS qos = MQTT::QOS0;
    auto func = [this](MQTT::MessageData& md){this->speedMessageReceived(md);};
    client.subscribe(sub_topic, qos, (MQTTClient::messageHandler)&func);

    while(true){
        client.yield(100);
    }
}