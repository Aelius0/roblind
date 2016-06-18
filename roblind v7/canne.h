#ifndef CANNE_H
#define CANNE_H
#include"Adafruit_ADS1015.h"
#include"capteurForce.h"
#include "capteurpresence.h"
#include"vibreur.h"
#include"joystick.h"
#include"wiringPi.h"
#include "wiringPiI2C.h"
#include "buzzer.h"

struct configCanne
{
    configJoystick confJoystick;
    int numPinVibreur;
    analogIn numCapteurPresence;
    analogIn numCapteurForce;
    int numPinBuzzer;
    //int numBouttonStop;
};

class CCanne
{
private:
    Adafruit_ADS1015 m_ADS1015;
public:
    CVibreur *m_vibreur;
    CCapteurForce *m_capteurForce;
    CCapteurPresence *m_capteurPresence;
    CBuzzer *m_buzzer;

    Cjoystick *m_joystick;
public:
    CCanne(configCanne config);
    void vibrer();

};

#endif // CANNE_H
