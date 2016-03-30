#include <QCoreApplication>
#include "Robot.h"
#include "../Moteurs/ThreadMoteur.h"
#include "../Capteurs/ThreadCapteurs.h"

enum sig {
    NONE = 0,
    VIBRER = 1,
    BIP = 2
};

sig sendSignal = NONE;

void react_bouton_arret() {
    if (active) {
//        baseRoulante.powerdown(); // TODO : argument ?
        delete path;
        path = new queue< vector<int> >;
        active = 0;
    } else {
        robot.calculer_direction();
        active = 1;
    }
}

void react_held() {
    robot.calculer_direction();
    active = 1;
    sendSignal = NONE;
}

void react_unheld() {
    robot.stop();
    active = 0;
    sendSignal = BIP;
}

// Thread principal
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    wiringPiSetup();
    robot();
    active = 1;
    pthread_create(&motThread, NULL, ThreadMoteur, NULL);
    pthread_create(&capteurThread, NULL, ThreadCapteur, NULL);

/*
    pinMode(9, OUTPUT);
    pinMode(2, INPUT);
    pullUpDnControl(2, PUD_UP);
*/
//    int test_event = wiringPiISR(2, INT_EDGE_BOTH, &react);
    
    while(true)
    {
        switch(sendSignal) {
            case VIBRER:
                robot.m_canne->vibrer();
                break;
            case BIP:
                // TODO : buzzer
                delay(1000);
                break;
        }
//        digitalWrite(9, !digitalRead(2));
//        cout << robot.x << endl;
        delay(10);
    }

    exit (0);
    return a.exec();
}
