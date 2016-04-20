#ifndef RELAY_H
#define RELAY_H
#include <wiringPi.h>

class CRelay
{
private:
    int m_numPin;
public:


    CRelay(int numPin);
    void TurnOn();
    void TurnOff();
};

#endif // RELAY_H
