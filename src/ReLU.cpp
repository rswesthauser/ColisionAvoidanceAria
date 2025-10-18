/*
    Muito usada em redes profundas
*/
#include "ReLU.h"


float ReLU::activate(float x) {
    return (x > 0.0f) ? x : 0.0f;
}

float ReLU::derivative(float x) {
    return (x > 0.0f) ? 1.0f : 0.0f;
}
    
    