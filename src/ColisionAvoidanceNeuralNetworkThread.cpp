#include "ColisionAvoidanceNeuralNetworkThread.h"
#include "Config.h"
#include <iostream>

ColisionAvoidanceNeuralNetworkThread::ColisionAvoidanceNeuralNetworkThread(PioneerRobot *_robo, NeuralNetwork *_neuralNetwork)
{
    this->robo = _robo;
    this->neuralNetwork = _neuralNetwork;
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
    ArLog::log(ArLog::Normal, "Colision Avoidance: tratamentoRna");
    //neuralNetwork.treinarValidar();
    
    /*if(robo->robot.isHeadingDone())
    {
        neuralNetwork.loop(int sonar[0] , int sonar[1] , int sonar[2] , int sonar[3] , int sonar[4] , int sonar[5] , int sonar[6] , int sonar[7]);
    }*/
}

