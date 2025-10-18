#include "ClassRobo.h"
#include "Aria.h"
#include <iostream>
#include "Config.h"
#include "Colisionavoidancethread.h"
#include "Wallfollowerthread.h"
#include "Sonarthread.h"
#include "Laserthread.h"

PioneerRobot *robo;

int main(int argc, char **argv)
{
    int sucesso;

    //Simulador
    robo = new PioneerRobot(ConexaoSimulacao,"",&sucesso);

    //Radio
    //robo = new PioneerRobot(ConexaoRadio,"192.168.1.11",&sucesso);
    //cout << "Sucesso = " << sucesso << "\n";

    ColisionAvoidanceThread colisionAvoidanceThread(robo);
    WallFollowerThread wallFollowerThread(robo);
    SonarThread sonarReadingThread(robo);
    LaserThread laserReadingThread(robo);

    ArLog::log(ArLog::Normal, "Colision Avoidance thread ...");
    colisionAvoidanceThread.runAsync();

    
    ArLog::log(ArLog::Normal, "Wall Following thread ...");
    //wallFollowerThread.runAsync();

    ArLog::log(ArLog::Normal, "Sonar Readings thread ...");
    sonarReadingThread.runAsync();

    //ArLog::log(ArLog::Normal, "Laser Readings thread ...");
    //laserReadingThread.runAsync();

    robo->robot.waitForRunExit();
    
    Aria::exit(0);
}
