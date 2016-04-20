#include <QCoreApplication>
//#include "baseroulante.h"
#include "roblind.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    configRoblind config;

    config.numPinIR= 27;

    config.confCanne.confJoystick.x=AIN0;
    config.confCanne.confJoystick.y=AIN1;
    config.confCanne.confJoystick.numPinBouton=0;
    config.confCanne.numPinVibreur=29; //à attribuer
    config.confCanne.numPinBuzzer=25; // à attribuer
    config.confCanne.numCapteurForce=AIN3;
    config.confCanne.numCapteurPresence=AIN2;

    strcpy(config.confBaseRoulante.numPort,"/dev/ttyAMA0");
    config.confBaseRoulante.numPinRelai=20;

    CRoblind robot(config);

   /* while(1)
    {
        int v;
        v = robot.m_canne->m_capteurPresence->getValue();

        cout<< v << endl;
        if(!robot.detecterObst() && robot.captPresence())
        {
            robot.allerEn();
        }else
        {
           //robot.m_baseRoulante->setVitesse(0,avancer);
           robot.m_baseRoulante->stop();
        }
    } */

    robot.m_baseRoulante->setVitesse(km0,avancer);


    exit (0);
    return a.exec();
}
