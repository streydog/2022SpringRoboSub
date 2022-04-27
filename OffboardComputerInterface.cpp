#include "OffboardComputerInterface.h"
#include <string>
#include "Robot.h"
#include <functional>

OffboardComputerInterface::OffboardComputerInterface(){
    // TODO: initializations or something? 
    client = new MQTTClient(&socket);
}

OffboardComputerInterface::~OffboardComputerInterface(){
    delete client;
}

float OffboardComputerInterface::get_speed(){
    mutex.lock();
    float temp_speed = speed;
    mutex.unlock();

    return temp_speed;
}

void OffboardComputerInterface::set_speed(float speed){
    mutex.lock();
    this->speed = speed;
    mutex.unlock();
}

void OffboardComputerInterface::initialization_sequence() {
    eth.set_network("192.168.0.9", "255.255.255.0", "192.168.0.1");

    nsapi_error_t status = eth.connect();

    eth.get_ip_address(&thisAddress);

    computerAddress.set_ip_address("192.168.0.10");
    computerAddress.set_port(1883);
    
    socket.open(&eth);
    int a = socket.connect(computerAddress);

    MQTTPacket_connectData conectData = MQTTPacket_connectData_initializer;

    int b = client->connect(conectData);

    thread.start(callback([this](){this->listen();}));
}

void OffboardComputerInterface::reset_sequence() {

}

void speed_mqtt_callback(MQTT::MessageData &md){
    MQTT::Message &message = md.message;
    char* incomingMessage = (char*)malloc(message.payloadlen + 1);
    strcpy(incomingMessage, (char*)md.message.payload);
    incomingMessage[message.payloadlen] = '\0';

    float speed = stof(incomingMessage);

    offboardComputerInterface.set_speed(speed);
    
    free(incomingMessage);
}

void OffboardComputerInterface::speed_message_received(MQTT::MessageData &md){
    MQTT::Message &message = md.message;
    char* incomingMessage = (char*)malloc(message.payloadlen + 1);
    strcpy(incomingMessage, (char*)md.message.payload);
    incomingMessage[message.payloadlen] = '\0';

    float speed = stof(incomingMessage);

    offboardComputerInterface.set_speed(speed);
    
    free(incomingMessage);
}

void OffboardComputerInterface::listen(){
    const char* sub_topic = "test/speed";
    MQTT::QoS qos = MQTT::QOS0;

    //std::function<void(MQTT::MessageData)> func = [this](MQTT::MessageData &md){this->speed_message_received(md);};

    client->subscribe(sub_topic, qos, speed_mqtt_callback);

    while(true){
        client->yield(500);
    }
}