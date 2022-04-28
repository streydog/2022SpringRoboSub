#include "HardwareRegistry.h"

HardwareRegistry::HardwareRegistry(){}

// I have no clue if this will work
void HardwareRegistry::initialize_all(){
    Thread* threads[hardware.size()];
    for(int i = 0; i<hardware.size(); i++){
        threads[i] = new Thread();
        Initializable* peripheral = hardware.at(i);
        threads[i]->start(callback(peripheral, &Initializable::initialize));
        printf("Looping");
    }
    for(int i = 0; i<hardware.size(); i++){
        threads[i]->join();
        delete(threads[i]);
    }
}

void HardwareRegistry::add(Initializable* periphery){
    hardware.push_back(periphery);
}