#include "capteurpresence.h"
#include "iostream"

using namespace std;

CCapteurPresence::CCapteurPresence(Adafruit_ADS1015 * ads1015, unsigned char channel)
{
    //wiringPiSetup();
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
{   int valeur = 0 ;
    bool retour = false;
    valeur = getValue();
//    cout << valeur << endl;
   if(valeur > 25)
   {
       retour = true;

   }
//     cout << retour << endl;
   return retour;
}
