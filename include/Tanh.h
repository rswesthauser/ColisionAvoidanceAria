#ifndef TANH_H
#define TANH_H

#include "ActivationFunction.h"
#include <cmath>

class Tanh : public ActivationFunction {
public:
    float activate(float x) override;

    float derivative(float x) override;
};

#endif