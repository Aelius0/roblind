#include "AX13.h"


AX13::AX13(int pin1 , int pin2)
{
    //if(wiringPiSetup() == -1)
      //  exit (1) ;
    m_pinServo1 = pin1;
    m_pinServo2 = pin2;



}
void AX13::setAngle()
{
    pinMode(m_pinServo1,OUTPUT);
    digitalWrite(m_pinServo1,LOW);
    softPwmCreate(m_pinServo1,0,200);

    pinMode(m_pinServo2,OUTPUT);
    digitalWrite(m_pinServo2,LOW);
    softPwmCreate(m_pinServo2,0,200);

    for(int i = 0; i < 100; i += 5)
    {
        softPwmWrite(m_pinServo1, i);
        delay(100);
    }

    for(int i = 0; i < 100; i += 5)
    {
        softPwmWrite(m_pinServo2, i);
        delay(100);
    }
}
