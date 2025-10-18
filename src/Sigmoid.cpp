#include "Sigmoid.h"
#include <cmath>

float Sigmoid::activate(float x) {
    return 1.0f / (1.0f + exp(-x));
}

float Sigmoid::derivative(float x) {
    float s = activate(x);
    return s * (1.0f - s);
}
    
    