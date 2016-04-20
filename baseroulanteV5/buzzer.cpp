#include "buzzer.h"

CBuzzer::CBuzzer(int pin)
{
    m_pin=pin;
    pinMode(pin,OUTPUT);
}

void CBuzzer::sonner()
{
    digitalWrite(m_pin,1);
}

void  CBuzzer::silence()
{
    digitalWrite(m_pin,0);
}

