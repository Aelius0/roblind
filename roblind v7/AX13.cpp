#include "AX13.h"


AX13::AX13(int pin1 , int pin2)
{
    m_pinServo1 = pin1;
    m_pinServo2 = pin2;
}

void AX13::setAngle()
{
    pinMode(m_pinServo1,OUTPUT);
    digitalWrite(m_pinServo1,LOW);
    softPwmCreate(m_pinServo1,0,200);

    /* UNUSED
    pinMode(m_pinServo2,OUTPUT);
    digitalWrite(m_pinServo2,LOW);
    softPwmCreate(m_pinServo2,0,200);
    */

    while (true) {
//        softPwmWrite(m_pinServo1, 14);
        softPwmWrite(m_pinServo1, 10);
        delay(180);
//        softPwmWrite(m_pinServo1, 05);
        softPwmWrite(m_pinServo1, 14);
        delay(180);
    }

}

void* AX13::moveLidar(void* servomoteur) {
    AX13* servo = static_cast<AX13*>(servomoteur);
    servo->setAngle();

    return 0;
}
