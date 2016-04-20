#ifndef JOYSTICK_H
#define JOYSTICK_H
#include"Adafruit_ADS1015.h"
#include"wiringPi.h"

enum position
{
    haut=0,bas,gauche,droite,centre,diagDroit,diagGauche
};
struct configJoystick
{
    analogIn x;
    analogIn y;
    int numPinBouton;
};

class Cjoystick
{
    Adafruit_ADS1015 * m_ads;
    unsigned char m_adcX;
    unsigned char m_adcY;
    int m_pinBouton;

public:
    Cjoystick(Adafruit_ADS1015 *ads1015, analogIn x, analogIn y,int pinBouton=0);
    int getX();
    int getY();
    position getPosition();
    bool isPressed();
};

#endif // JOYSTICK_H
