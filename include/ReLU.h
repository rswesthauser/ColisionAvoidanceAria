#ifndef RELU_H
#define RELU_H

#include "ActivationFunction.h"

class ReLU : public ActivationFunction {
public:
    float activate(float x) override;

    float derivative(float x) override;
};

#endif
