/*
    Evita a "morte" de neuronios
*/
#include "LeakyReLU.h"

float LeakyReLU::activate(float x) {
    return (x > 0.0f) ? x : 0.01f * x;
}

float LeakyReLU::derivative(float x) {
    return (x > 0.0f) ? 1.0f : 0.01f;
}
