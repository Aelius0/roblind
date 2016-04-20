#include "capteurForce.h"

CCapteurForce::CCapteurForce(Adafruit_ADS1015 * ads1015, unsigned char channel)
{
    wiringPiSetup();
    m_ads1015=ads1015;
    m_adc=channel;

}

int CCapteurForce::getValue()
{
    int value=0;
    value=m_ads1015->readADC_SingleEnded(m_adc);
    return value;
}

vitesse CCapteurForce::calculVitesse()
{
    int pression = 0,pas=400;
    vitesse v = km0;
    pression = getValue();

    if(pression > pas && pression < pas*2)
        v=km05;

    if(pression > pas*2 && pression < pas*3)
        v=km1;

    if(pression > pas*3 && pression < pas*4)
        v=km15;

    if(pression > pas*4 && pression < pas*5)
        v=km2;

    if(pression > pas*5 && pression < pas*6)
        v=km25;

    if(pression > pas*6 && pression < pas*7)
        v=km3;

    if(pression > pas*7 && pression < pas*8)
        v=km35;

    if(pression > pas*8 && pression < pas*9)
        v=km4;

    if(pression > pas*9 && pression < pas*10)
        v=km45;

    if(pression > pas*10)
        v=km5;

    return v;


}
