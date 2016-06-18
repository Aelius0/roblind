#ifndef CAPTEURFORCE_H
#define CAPTEURFORCE_H
#include "Adafruit_ADS1015.h"
#include"kangaroo.h"

class CCapteurForce
{
private:
    Adafruit_ADS1015 *m_ads1015;
    unsigned char m_adc;
public:
    CCapteurForce(Adafruit_ADS1015 * ads1015, unsigned char channel);
    int getValue();
    vitesse calculVitesse();


};

#endif // CAPTEURFORCE_H
