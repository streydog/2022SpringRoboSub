#include "Initializable.h"

Initializable::Initializable(){
    initialized = false;
    sequenceStarted = false;
}

void Initializable::initialize(){
    if(sequenceStarted == false){
        sequenceStarted = true;
        initialization_sequence();
        initialized = true;
    }
}

bool Initializable::is_initialized() const{
    return initialized;
}

void Initializable::reset(){
    reset_sequence();
    initialized = false;
    sequenceStarted = false;
}

void Initializable::initialization_sequence(){
    // nothing
    printf("Default init pls override");
}

void Initializable::reset_sequence(){
    // nothing
}