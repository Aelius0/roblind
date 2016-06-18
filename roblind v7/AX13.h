#ifndef AX13_H
#define AX13_H
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <softPwm.h>
#include <iostream>
struct configAX13
{
    int pinServo1;
    int pinServo2;
};


class AX13
{   
public :
    AX13(int pin1, int pin2);
    void setAngle();
    int m_pinServo1;
    int m_pinServo2;

    static void* moveLidar(void* servo);
};


#endif // AX13_H
