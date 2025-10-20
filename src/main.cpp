#include "ClassRobo.h"
#include "Aria.h"
#include <iostream>
#include "Config.h"
#include "Colisionavoidancethread.h"
#include "Wallfollowerthread.h"
#include "Sonarthread.h"
#include "Laserthread.h"
#include "ColisionAvoidanceNeuralNetworkThread.h"

PioneerRobot *robo;
NeuralNetwork *neuralNetwork;

int main(int argc, char **argv)
{
    int sucesso;

    //Simulador
    robo = new PioneerRobot(ConexaoSimulacao,"",&sucesso);

    neuralNetwork = new NeuralNetwork();

    //Radio
    //robo = new PioneerRobot(ConexaoRadio,"192.168.1.11",&sucesso);
    //cout << "Sucesso = " << sucesso << "\n";

    ArLog::log(ArLog::Normal, "Criando as theads...");
    //ColisionAvoidanceThread colisionAvoidanceThread(robo);
    ColisionAvoidanceNeuralNetworkThread colisionAvoidanceNeuralNetworkThread(robo, neuralNetwork);
    //WallFollowerThread wallFollowerThread(robo);
    SonarThread sonarReadingThread(robo);
    //LaserThread laserReadingThread(robo);

    ArLog::log(ArLog::Normal, "Sonar Readings thread ...");
    sonarReadingThread.runAsync();

    //ArLog::log(ArLog::Normal, "Laser Readings thread ...");
    //laserReadingThread.runAsync();

    //ArLog::log(ArLog::Normal, "Colision Avoidance thread ...");
    //colisionAvoidanceThread.runAsync();

    ArLog::log(ArLog::Normal, "Colision Avoidance Neural Network thread ...");
    //colisionAvoidanceNeuralNetworkThread.runAsync();
    
    //ArLog::log(ArLog::Normal, "Wall Following thread ...");
    //wallFollowerThread.runAsync();

    robo->robot.waitForRunExit();
    
    Aria::exit(0);
}
