#include "OCI.h"

#include <string>
#include <functional>
#include "Robot.h"

using namespace OCI;

Thread thread;
EthernetInterface eth;
SocketAddress thisAddress;
SocketAddress computerAddress;
TCPSocket socket;
MQTTClient* client;
Mutex OCI_mutex;

volatile float speed;

/**
 *  TODO: There's probably more than one speed.
 */
float OCI::get_speed(){
    OCI_mutex.lock();
    float tempSpeed = speed;
    OCI_mutex.unlock();

    return tempSpeed;
}

/**
 *  Send an error message to the offboard computer. 
 */
void send_error_message(const char* error){
    // Print on serial console, if there's one connected. 
    fprintf(stderr, "%s\n", error);

    // Publish the error message to the offboard computer.
    MQTT::Message message;
    message.payload = (char*)error;
    client->publish("/error", message);
}

/**
 *  This function is the callback that should be passed into the subscribe() function for mqtt.
 * 
 *  When messages are published to /robot/#, this callback will be called and the message can be handled. 
 */
void mqtt_callback(MQTT::MessageData &md){
    string topic = md.topicName.cstring;

    MQTT::Message &message = md.message;
    char* incomingMessage = (char*)malloc(message.payloadlen + 1);
    strcpy(incomingMessage, (char*)md.message.payload);
    incomingMessage[message.payloadlen] = '\0';

    if(topic == "/robot/speed"){
        float data = stof(incomingMessage);

        OCI_mutex.lock();
        speed = data;
        OCI_mutex.unlock();
    }else if(topic == "/robot/other"){ // TODO: Add whatever topics we need to subscribe to.

    }else{
        send_error_message("The robot received an invalid callback.");
    }
    
    free(incomingMessage);
}

/**
 *  Begin listening for mqtt messages from the offboard computer. 
 */
void listen(){
    // Subscribe to all topics under /robot
    const char* sub_topic = "/robot/#";

    client->subscribe(sub_topic, MQTT::QOS2, mqtt_callback);

    while(true){
        client->yield(500);
    }
}

/**
 *  Initialize the mqtt interface with the offboard computer. 
 */
void OCI::start() {
    eth.set_network("192.168.0.9", "255.255.255.0", "192.168.0.1");

    nsapi_error_t status = eth.connect();

    eth.get_ip_address(&thisAddress);

    computerAddress.set_ip_address("192.168.0.10");
    computerAddress.set_port(1883);
    
    socket.open(&eth);
    socket.connect(computerAddress);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    client->connect(connectData);

    thread.start([]{listen();});
}

/**
 *  Stop?
 */
void stop() {

}