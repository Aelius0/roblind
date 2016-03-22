#ifndef KANGAROO_H
#define KANGAROO_H
#include "serialport.h"
#include "stdlib.h"
#include <QString>
enum mode
{
    moteur1=0x31,moteur2=0x32,drive=0x44,turn=0x54
};
enum unite
{
    mm=142,cm=14231,m=1423077 // mm = 14 ?
};

class Ckangaroo
{
public:
    Ckangaroo(const char * portName);

    bool allerEn (int distance , int speed, unite u);
    bool tourner (double angle , double speed);
    void setMixedMode(double distanceDrive , double speedDrive , double rotation , double speedTurn);

    int getPosition (mode m,int &position);

    void getSpeed (mode m);
    double getPositionMax (mode m);
    double getPositonMin (mode m);
    bool powerdown(const char* m);



    CSerialPort m_serialPort;
    bool m_isOpened;

     bool init();
     bool start(mode m);

};

#endif // KANGAROO_H
