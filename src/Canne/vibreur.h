#ifndef CMOTEUR_H
#define CMOTEUR_H
#include"wiringPi.h"
#include"vibreur.h"

class CVibreur
{
private:
    int m_pin;

public:
    CVibreur(int pin);
    void vibrer();
    void arreter();

};

#endif // CMOTEUR_H
