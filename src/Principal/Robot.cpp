
//class header file
#include "Robot.h"
#include "../Moteurs/ThreadMoteur.h"
#include "../Capteurs/ThreadCapteurs.h"

using namespace std;
pthread_t motThread, capteurThread;
Robot robot;


void Robot::avancerRobot(int dist) {
    double angle = robot.o * 2 * math.pi / 360;
    x += math.cos(angle)*dist;
    y += math.sin(angle)*dist;

    // CHECK SORTIE DE GRILLE => translation (ne pas oublier de translater le robot avec)
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


bool Robot::obstacleDevant(int dist = 10)
{
    double angle = robot.o * 2 * math.pi / 360;
    int dx = static_cast<int>(math.cos(angle)*dist);
    int dy = static_cast<int>(math.sin(angle)*dist);
    if (o < 180) {
        if (o < 90 || o > 270) {
            for (int i = 0; i < dx; ++i)
                for (int j = 0; j < dy; ++j)
                    if (grille_espace.get(x + i, y + j) > 0)
                        return true;
        }
        else {
            for (int i = dx; i < 0; ++i)
                for (int j = 0; j < dy; ++j)
                    if (grille_espace.get(x + i, y + j) > 0)
                        return true;
        }
    } else {
        for (int j = 0; j < dy; ++j)
            if (o < 90 || o > 270) {
                for (int i = 0; i < dx; ++i)
                    if (grille_espace.get(x + i, y + j) > 0)
                        return true;
            } else {
                for (int i = dx; i < 0; ++i)
                    if (grille_espace.get(x + i, y + j) > 0)
                        return true;
            }
    }
}
