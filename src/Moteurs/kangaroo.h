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
    mm=38,cm=383,m=38312,degre=97
};

class Ckangaroo
{

public:
    Ckangaroo(const char * portName);
    ~Ckangaroo();

    bool allerEn (int distance , int speed, unite u);
    bool tourner (int angle);

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
