#include "canne.h"


CCanne::CCanne(configCanne config)
{
    //wiringPiSetup();
    m_joystick= new Cjoystick(&m_ADS1015,config.confJoystick.x,config.confJoystick.y, config.confJoystick.numPinBouton);
    m_vibreur= new CVibreur(config.numPinVibreur);
    m_buzzer = new CBuzzer(config.numPinBuzzer);


    m_capteurPresence= new CCapteurPresence(&m_ADS1015,config.numCapteurPresence);
    m_capteurForce= new CCapteurForce(&m_ADS1015,config.numCapteurForce);


}
