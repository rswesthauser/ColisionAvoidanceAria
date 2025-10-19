#include "ColisionAvoidanceNeuralNetworkThread.h"
#include "Config.h"
#include <iostream>

ColisionAvoidanceNeuralNetworkThread::ColisionAvoidanceNeuralNetworkThread(PioneerRobot *_robo)
{
    this->robo = _robo;
}

void* ColisionAvoidanceNeuralNetworkThread::runThread(void*)
{
    while(this->getRunningWithLock())
    {
        myMutex.lock();
        robo->getAllSonar(sonar);
        tratamentoRna();
        //ArUtil::sleep(1000);
        myMutex.unlock();
    }

    ArLog::log(ArLog::Normal, "Colision Avoidance.");
    return NULL;
}

void ColisionAvoidanceNeuralNetworkThread::waitOnCondition() { myCondition.wait(); }

void ColisionAvoidanceNeuralNetworkThread::lockMutex() { myMutex.lock(); }

void ColisionAvoidanceNeuralNetworkThread::unlockMutex() { myMutex.unlock(); }

void ColisionAvoidanceNeuralNetworkThread::tratamentoRna()
{
    neuralNetwork.treinarValidar();
    ArUtil::sleep(9999999999);
}

