/*
    Tanh (range -1 a 1, centrada em zero):
*/
#include "Tanh.h"


float activate(float x) {
    return tanh(x);
}

float derivative(float x) {
    float t = tanh(x);
    return 1.0f - t * t;
}
 
    