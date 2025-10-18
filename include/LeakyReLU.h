#ifndef LEAKY_RELU_H
#define LEAKY_RELU_H

#include "ActivationFunction.h"

class LeakyReLU : public ActivationFunction {
public:
    float activate(float x) override;
    float derivative(float x) override;
};

#endif
