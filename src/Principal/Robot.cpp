
//class header file
#include "Robot.h"
#include "../Moteurs/ThreadMoteur.h"
#include "../Capteurs/ThreadCapteurs.h"

using namespace std;
pthread_t motThread, capteurThread;
Robot robot;


void Robot::avancerRobot(int dist) {
    double angle = robot.o * 2 * math.pi / 360;
}

int main(int argc, char *argv[])
{
    cout << "Main robot code" << endl;
    wiringPiSetup();
    grille = new Grille(resolutionX,resolutionY,tailleX,tailleY);
    pthread_create(&motThread, NULL, ThreadMoteur, NULL);
    pthread_create(&capteurThread, NULL, ThreadCapteur, NULL);

    pinMode(9, OUTPUT);
    pinMode(2, INPUT);
    pullUpDnControl(2, PUD_UP);

    //int test_event = wiringPiISR(2, INT_EDGE_BOTH, &react);
    
    while(true)
    {
//        digitalWrite(9, !digitalRead(2));
        cout << robot.x << endl;
        delay(1000);
    }

    return 0;
}



bool Robot::calculer_chemin(double direction)
{

}


