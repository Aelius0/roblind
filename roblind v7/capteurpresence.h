#ifndef CAPTEURPRESENCE_H
#define CAPTEURPRESENCE_H
#include "Adafruit_ADS1015.h"

class CCapteurPresence
{
private:
    Adafruit_ADS1015 *m_ads1015;
    unsigned char m_adc;
public:
    CCapteurPresence(Adafruit_ADS1015 * ads1015, unsigned char channel);
    int getValue();
    bool checkPresence();
};

#endif // CAPTEURPRESENCE_H
