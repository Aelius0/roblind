#include "capteurpresence.h"

CCapteurPresence::CCapteurPresence(Adafruit_ADS1015 * ads1015, unsigned char channel)
{
    wiringPiSetup();
    m_ads1015=ads1015;
    m_adc=channel;

}

int CCapteurPresence::getValue()
{
    int value=0;
    value=m_ads1015->readADC_SingleEnded(m_adc);
    return value;
}

bool CCapteurPresence::checkPresence()
{

    return getValue() > 25;
}
