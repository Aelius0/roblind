#ifndef KANGAROO_H
#define KANGAROO_H
#include "serialport.h"
#include <stdlib.h>

enum mode
{
    moteur1=0x31,moteur2=0x32,drive=0x44,turn=0x54
};

enum unite
{
    mm=4,cm=34,m=3395,degre=7
};

enum vitesse
{
    km0=0, km05=200,km1=400,km15=600,km2=800,km25=1000,km3=1200,km35=1400,km4=1600,km45=1800,km5=2000
};

class Ckangaroo
{

public:
    Ckangaroo(const char * portName);
    ~Ckangaroo();

    bool allerEn (vitesse v ,int distance , unite u);
    bool setVitesse (vitesse v);
    bool tourner (vitesse v,int angle);

    int getPosition (mode m,int &position);
    int getPositionMax (mode m, int &positionMax); // retourne le range de position maximal
    int getPositionMin (mode m , int &positionMin); //retourne le range de position minimal
    int getSpeed (mode m, int &speed); //retourne la vitesse


    CSerialPort m_serialPort;
    bool m_isOpened;

     bool init();
     bool start(mode m);
     bool powerdown(mode m);
     void downZero();


};

#endif // KANGAROO_H
