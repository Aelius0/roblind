#include "AX13.h"


AX13::AX13()
{
    if(wiringPiSetup() == -1)
        exit (1) ;
    pinMode(7,OUTPUT);
    digitalWrite(7, LOW);
    softPwmCreate(7,0,200);
}
void AX13::setAngle()
{

    for(int i = 0; i < 100; i += 5) {
        softPwmWrite(7, i);
        delay(100);
    }
}
