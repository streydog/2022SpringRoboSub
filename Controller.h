#include "Initializable.h"
#include <MQTTClientMbedOs.h>
#include "EthernetInterface.h"
#include <cstring>

class Controller : public Initializable{
    public:
        Controller();
        float getSpeed() const;
    protected:
        void initializationSequence() override;
        void resetSequence() override;
    private:
        Thread thread;
        MQTTClient client;
        void speedMessageReceived(MQTT::MessageData& md);
        void listen();
        float speed;
};