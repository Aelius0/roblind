
/*
 * File Type: Class body
 * Class: Robot
 */


//class header file
#include "Robot.h"
#include "../Moteurs/ThreadMoteur.h"
#include "../Capteurs/ThreadCapteurs.h"



using namespace std;
pthread_t motThread, capteurThread;
Robot robot;

int main(int argc, char *argv[])
{
    cout << "Main robot code" << endl;
    pthread_create(&motThread, NULL, ThreadMoteur, NULL);
    pthread_create(&capteurThread, NULL, ThreadCapteur, NULL);

    wiringPiSetup();
    
    
    pinMode(9, OUTPUT);
    pinMode(2, INPUT);
    pullUpDnControl(2, PUD_UP);

    //int test_event = wiringPiISR(2, INT_EDGE_BOTH, &react);
    
    while(true)
    {
//        digitalWrite(9, !digitalRead(2));
        cout << robot.x<< endl;
        delay(1000);
    }

    return 0;
}


void Robot::marquer_bloque(int x, int y)
{
    grille_espace[x][y] = 1;
}


void Robot::marquer_libre(int x, int y)
{
    grille_espace[x][y] = 0;
}


bool Robot::calculer_chemin(double direction)
{
//begin of modifiable zone(Cxx.Code)......C/b023ce1d-fe98-4d92-9cb1-a798f3f0c794
// TODO complete code
//end of modifiable zone(Cxx.Code)........E/b023ce1d-fe98-4d92-9cb1-a798f3f0c794

} // end calculer_chemin


