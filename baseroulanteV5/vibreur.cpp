#include "vibreur.h"

CVibreur::CVibreur(int pin)
{
    m_pin=pin;
    pinMode(pin,OUTPUT);
}

void CVibreur::vibrer()
{
    digitalWrite(m_pin,1);
}

void  CVibreur::arreter()
{
    digitalWrite(m_pin,0);
}

