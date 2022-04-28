#include "OCI.h"

#include <string>
#include "Robot.h"
#include <functional>

Thread thread;
EthernetInterface eth;
SocketAddress thisAddress;
SocketAddress computerAddress;
TCPSocket socket;
MQTTClient* client;
Mutex OCI_mutex;

float speed;

float get_speed(){
    OCI_mutex.lock();
    float temp_speed = speed;
    OCI_mutex.unlock();

    return temp_speed;
}

void speed_mqtt_callback(MQTT::MessageData &md){
    MQTT::Message &message = md.message;
    char* incomingMessage = (char*)malloc(message.payloadlen + 1);
    strcpy(incomingMessage, (char*)md.message.payload);
    incomingMessage[message.payloadlen] = '\0';

    float data = stof(incomingMessage);

    OCI_mutex.lock();
    speed = data;
    OCI_mutex.unlock();
    
    free(incomingMessage);
}

void listen(){
    const char* sub_topic = "test/speed";
    MQTT::QoS qos = MQTT::QOS0;

    //std::function<void(MQTT::MessageData)> func = [this](MQTT::MessageData &md){this->speed_message_received(md);};

    client->subscribe(sub_topic, qos, speed_mqtt_callback);

    while(true){
        client->yield(500);
    }
}

void start() {
    eth.set_network("192.168.0.9", "255.255.255.0", "192.168.0.1");

    nsapi_error_t status = eth.connect();

    eth.get_ip_address(&thisAddress);

    computerAddress.set_ip_address("192.168.0.10");
    computerAddress.set_port(1883);
    
    socket.open(&eth);
    socket.connect(computerAddress);

    MQTTPacket_connectData conectData = MQTTPacket_connectData_initializer;

    client->connect(conectData);

    thread.start([]{listen();});
}

void stop() {

}