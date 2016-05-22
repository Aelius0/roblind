#include "relay.h"

CRelay::CRelay(int numPin)
{
    //wiringPiSetup();
    m_numPin=numPin;
    pinMode(numPin,1);
}

void CRelay::TurnOn()
{
    digitalWrite(m_numPin , 1);


}

void CRelay::TurnOff()
{
    digitalWrite(m_numPin , 0);


}
