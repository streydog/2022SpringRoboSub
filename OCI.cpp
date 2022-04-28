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

/**
 *  Variable whos value can be synchronized from one thread to another.
 *  
 *  The "asynchronous" value is the value from the source thread.
 *  The "synchronous" value is the value for the destination thread. It will not change until update_value() is called.
 *  
 *  Designed to be a drop in replacement for the variable of the same type. Mutex is handled within the class.
 *  Assignment always changes the asynchronous value.
 *  Can be implicitly cast to variable of the type. This will always return the synchronous value.
 */
template<typename T>
class MQTTSynchronousVariable{
    public:
        MQTTSynchronousVariable(T value){
            async_value = value;
        }
        MQTTSynchronousVariable(){}
        void update_value(){
            OCI_mutex.lock();
            sync_value = async_value;
            OCI_mutex.unlock();
        }
        MQTTSynchronousVariable& operator = (T value){
            OCI_mutex.lock();
            async_value = value;
            OCI_mutex.unlock();
            return *this;
        }
        operator T (){
            OCI_mutex.lock();
            float temp_value = sync_value;
            OCI_mutex.unlock();
            return temp_value;
        }
    private:
        T sync_value;
        volatile T async_value;
};


MQTTSynchronousVariable<float> speed = 0.0;
MQTTSynchronousVariable<float> strafe = 0.0;
MQTTSynchronousVariable<float> rotation = 0.0;
MQTTSynchronousVariable<float> depth = 0.0;


/**
 *  Updates the values of all MQTTSynchronousVariables
 */
void OCI::update_values(){
    speed.update_value();
    strafe.update_value();
    rotation.update_value();
    depth.update_value();
}

/**
 *  TODO: There's probably more than one speed.
 */
float OCI::get_speed(){
    return speed;
}
float OCI::get_strafe(){
    return strafe;
}
float OCI::get_rotation(){
    return rotation;
}
float OCI::get_depth(){
    return depth;
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

    if(topic == "/robot/control/speed"){
        speed = stof(incomingMessage);
    }
    else if(topic == "/robot/control/strafe"){
        strafe = stof(incomingMessage);
    }
    else if(topic == "/robot/control/rotation"){
        rotation = stof(incomingMessage);
    }
    else if(topic == "/robot/control/depth"){ // TODO: Add whatever topics we need to subscribe to.
        depth = stof(incomingMessage);
    }
    else{
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
 *  TODO: Method to stop listening, disconnect from the broker, and close the socket
 */
void stop() {

}