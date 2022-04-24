#include "Initializable.h"

Initializable::Initializable(){
    initialized = false;
    sequenceStarted = false;
}

void Initializable::initialize(){
    if(sequenceStarted == false){
        sequenceStarted = true;
        initializationSequence();
        initialized = true;
    }
}

bool Initializable::isInitialized() const{
    return initialized;
}

void Initializable::reset(){
    resetSequence();
    initialized = false;
    sequenceStarted = false;
}