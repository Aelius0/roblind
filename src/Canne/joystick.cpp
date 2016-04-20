#include "joystick.h"
#include "iostream"

using namespace std;

Cjoystick:: Cjoystick(Adafruit_ADS1015 * ads1015, analogIn channelX, analogIn channelY, int pinBouton)
{
    wiringPiSetup();
    m_pinBouton=pinBouton;
    pinMode(m_pinBouton,INPUT);
   // pullUpDnControl(m_pinBouton,PUD_OFF);
    m_ads=ads1015;
    m_adcX=channelX;
    m_adcY=channelY;
}

int Cjoystick:: getX()
{
    int value=0;
    value=m_ads->readADC_SingleEnded(m_adcX);
    return value;
}

int Cjoystick:: getY()
{
    int value=0;
    value=m_ads->readADC_SingleEnded(m_adcY);
    return value;
}

position Cjoystick::getPosition()
{
    position p=centre;
    int x=0,y=0;

    x=getX();
    y=getY();



    if(x>1650 && x<1750 && y>1650 &&y<1750)
        p=centre;

    if(x>3200&& x<3400 && y>1650 &&y<1750)
        p=gauche;

    if(x>=0 && x<10 && y>1650 &&y<1750)
        p=droite;

    if(x>1650 && x<1750 && y>3200 &&y<3400)
        p=haut;

    if(x>1650 && x<1750 && y>=0 && y<10)
        p=bas;

   /* if(x>17 && x < 2454 && y>2354 && y < 2454)
        p=diagDroit;

    if(x>746 && x < 846 && y>2354 && y < 2454)
        p=diagGauche; */

    //droite = bas
    // gauche = haut


    return p;
}

bool Cjoystick:: isPressed()
{
    int retour=digitalRead(m_pinBouton);
    return (bool)!retour;
}

