#include "roblind.h"



CRoblind::CRoblind(configRoblind config)
{
    m_canne = new CCanne(config.confCanne);
    m_baseRoulante= new Cbaseroulante(config.confBaseRoulante.numPort,config.confBaseRoulante.numPinRelai);
    m_lidarLite = new lidarLite();
    m_capteurIR = new CapteurIR();
    m_etat = arret;
    wiringPiSetup();
    pinMode(config.numPinIR,INPUT);
    m_config=config;


}

bool CRoblind::allerEn()
{
   position p;

   bool stop = false;


   m_vitesse=m_canne->m_capteurForce->calculVitesse();
   p = m_canne->m_joystick->getPosition();
   stop = m_canne->m_joystick->isPressed();
   //cout<<"Position: "<<p<<endl;

   if ( p == 0 && m_etat != avance ) // avancer
   {

        m_baseRoulante->setVitesse(m_vitesse,avancer);
        m_etat = avance;


   }



    if ( p == 1 && m_etat != recule ) // reculer
    {

        m_baseRoulante->setVitesse(m_vitesse,reculer);
        m_etat = recule;



    }


    if (p == 2 && m_etat != gogauche) //aller à gauche 90°
    {
        m_baseRoulante->tourner(m_vitesse,90);
        m_etat = gogauche;


    }




    if (p == 3 && m_etat != godroite) //aller a droite 90°
    {

        m_baseRoulante->tourner(m_vitesse,-90);
        m_etat = godroite;

    }

    if (p == 6 && m_etat != anglegauche) //angle gauche 45°
    {
        m_baseRoulante->tourner(m_vitesse,45);
        m_etat = anglegauche;


    }

    if (p == 5 && m_etat != angledroit) //angle droit 45°
    {
        m_baseRoulante->tourner(m_vitesse,-45);
        m_etat = angledroit;


    }




    if (stop == true) //stop
    {

        m_baseRoulante->stop();

    }

}


bool CRoblind::detecterObst()
{

    bool retour=false;


    if(digitalRead(m_config.numPinIR) == 0)
    {
         m_etat = arret;
         m_baseRoulante->m_status=stope;
         retour=true;
    }

   return retour;

}

bool CRoblind::captPresence()
{
    bool ok = m_canne->m_capteurPresence->checkPresence();

    if (!ok)
    {
        m_etat = arret;
        m_baseRoulante->m_status=stope;

    }

    return ok;


}
