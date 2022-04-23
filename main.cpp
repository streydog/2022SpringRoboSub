

/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */


#include <csetjmp>
#include <cstring>
#include <stdio.h>

#include "mbed.h"
#include "mpu6050.h"
#include "motor_controller.h"

#include <MQTTClientMbedOs.h>

#include "EthernetInterface.h"

int x_vel = 0;
int y_vel = 0;
int z_vel = 0;



UnbufferedSerial pc(USBTX, USBRX); // tx, rx
PwmOut led(PF_9);

int milliseconds = 1500;
float period = 0.02f;
float output = (milliseconds/1000000.0f)/period;
char c;

void message_arrived(MQTT::MessageData& md){
    printf("message awwived\n");
    MQTT::Message &message = md.message;
    //char* incoming_message = (char*)malloc(message.payloadlen + 1);
    //strcpy(incoming_message, (char*)md.message.payload);
    //incoming_message[message.payloadlen] = '\0';

    //printf("%s\n", incoming_message);
    //free(incoming_message);

    char c = *((char*)md.message.payload);
    if((c == 'u') && (milliseconds < 2000)) {
        milliseconds += 5;
    }
    if((c == 'd') && (milliseconds > 1000)) {
        milliseconds -= 5;
    }
    if(c == 'o') {
        milliseconds = 1500;
    } 
    
    int temp = 1000000.0f * output;
    printf("%d\n", temp);

    output = (milliseconds/1000000.0f)/period;
    led.write(output);
}


int main()
{
    printf("hello world, dude\n");

    /**
    MPU6050 mpu6050(D14, D15);

    //uint8_t test = mpu6050.testConnection();

    //printf("test: %d\n", test);

    int avg_x = 0;
    int avg_y = 0;
    int avg_z = 0;

    printf("keep the box steady (preferably on a table)\nthe calibration will begin in 3s\n");

    ThisThread::sleep_for(3s);

    printf("calibration started\n");

    for(int i = 0; i < 100; i++){
        int16_t accel_data[3];
        mpu6050.readAccelData(accel_data);

        avg_x += accel_data[0];
        avg_y += accel_data[1];
        avg_z += accel_data[2];

        ThisThread::sleep_for(10ms);
    }

    avg_x = avg_x / 100;
    avg_y = avg_y / 100;
    avg_z = avg_z / 100;

    printf("calibration finished\n");

    printf("begin moving around\ndata will start collecting in 3s\n");

    ThisThread::sleep_for(3s);

    printf("----------------test.csv------------------\n");
    printf("index, x_vel\n");

    for(int i = 0; i < 1000; i++){
        int x_accel = 0;
        int y_accel = 0;
        int z_accel = 0;

        for(int j = 0; j < 3; j++){
            int16_t accel_data[3];
            mpu6050.readAccelData(accel_data);

            x_accel += (accel_data[0]-avg_x);
            y_accel += (accel_data[1]-avg_y);
            z_accel += (accel_data[2]-avg_z);
        }
        x_accel /= 3;
        y_accel /= 3;
        z_accel /= 3;

        if(abs(x_accel) > 300)
            x_vel = x_vel + (x_accel/200);

        printf("%d, %d\n", i, x_vel);

        ThisThread::sleep_for(5ms);
    }

    printf("------------------end-------------------\n");

    printf("avg_x: %d\n", avg_x);
    printf("avg_y: %d\n", avg_y);
    printf("avg_z: %d\n", avg_z);
    */

    
    EthernetInterface eth;
    eth.set_network("192.168.0.9", "255.255.255.0", "192.168.0.1");

    nsapi_error_t status = eth.connect();

    printf("status: %d\n", status);

    SocketAddress this_address;
    eth.get_ip_address(&this_address);

    printf("This IP Address: %s\n", this_address.get_ip_address());

    /*
    UDPSocket socket;
    socket.open(&eth);
    */

    SocketAddress computer_address;
    computer_address.set_ip_address("192.168.0.10");
    computer_address.set_port(1883);

    /*
    char buff[] = "hi from stm32";
    
    for(;;){
        if (0 > socket.sendto(computer_address, buff, sizeof(buff))) {
            printf("Error sending data\n");
            return -1;
        }


        ThisThread::sleep_for(500ms);
    }
    

    socket.close();
    //eth.disconnect();
    printf("Done\n");

    */

    
    TCPSocket socket;
    MQTTClient client(&socket);
    int a = socket.open(&eth);


    int b = socket.connect(computer_address);
    printf("a: %d\n", a);
    printf("b: %d\n", b);

    //if(b == NSAPI_ERROR_NO_CONNECTION){
    //    printf("NSAPI_ERROR_NO_CONNECTION\n");
    //}

    //for(;;);

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;

    client.connect(connect_data);

    char* sub_topic = "test/2";

    MQTT::QoS qos = MQTT::QOS0;

    int sub_return = client.subscribe(sub_topic, qos, message_arrived);
    if(sub_return != 0){
        printf("sub_return: %d\n", sub_return);
    }else{
        printf("we good\n");
    }

    char* topic = "test/1";

    MQTT::Message message;
    char buf[100] = "hello from the stm32"; 
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadlen = strlen(buf)+1;
    int rt = client.publish(topic, message);

    //client.yield(1000000);

    /*
    setup_m1();
    for(;;){
        ThisThread::sleep_for(500ms);
        printf("Hello!\n");
    }
    */

    
	printf("Press 'u' to turn LED1 brightness up, 'd' to turn it down\n");

    led.period(period);  // 4 second period
    led.write(output);

    for(;;){
        client.publish(topic, message);
        client.yield(1000);
    }

    /*
    for(;;) {
        client.yield(30);
        output = (milliseconds/1000000.0f)/period;
        led.write(output);
        if(pc.read(&c, 1)){
            if((c == 'u') && (milliseconds < 2000)) {
                milliseconds += 5;
            }
            if((c == 'd') && (milliseconds > 1000)) {
                milliseconds -= 5;
            }
            if(c == 'o') {
                milliseconds = 1500;
            } 
            

            int temp = 1000000.0f * output;
            printf("%d\n", temp);

        }
    }
    */

    

    return 0;

}


