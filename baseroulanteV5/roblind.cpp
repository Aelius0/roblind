#include "roblind.h"

pthread_mutex_t CRoblind::mut;
pthread_cond_t CRoblind::cond;

CRoblind::CRoblind(configRoblind config)
{
    wiringPiSetup();
    m_canne = new CCanne(config.confCanne);
    m_baseRoulante= new Cbaseroulante(config.confBaseRoulante.numPort,config.confBaseRoulante.numPinRelai);
    m_lidarLite = new lidarLite();
    m_capteurIR = new CapteurIR();
    m_pinServo = new AX13(config.confAX13.pinServo1,config.confAX13.pinServo2);
    m_gps=new cgps();
    m_etat = arret;
    pinMode(config.numPinIR,INPUT);
    m_config=config;

    m_grille = new Grille(5, 1000);

    enEvitement = false;
}

// Relai statique vers boucleMouvement
void* CRoblind::executerMouvement(void* robot) {
    CRoblind* roblind = static_cast<CRoblind*>(robot);
    etatRoblind state = roblind->m_etat;
    while (true) {
        if (nouvelOrdre) {
            nouvelOrdre = false;
            switch (roblind->m_etat) {
            case avance:
                m_baseRoulante->setVitesse(km1,avancer);
                break;
            case recule:
                m_baseRoulante->setVitesse(km1,reculer);
                break;
            case gogauche:
                m_baseRoulante->tourner(km1,90);
                break;
            case godroite:
                m_baseRoulante->tourner(km1,-90);
                break;
            case anglegauche:
                m_baseRoulante->tourner(km1,45);
                break;
            case angledroit:
                m_baseRoulante->tourner(km1,-45);
                break;
            }
            //            pthread_mutex_lock(&mut);
            //            roblind->m_etat = arret;
            //            pthread_mutex_unlock(&mut);
        }
    }
    return 0;
}

void CRoblind::boucleMouvement() {
    while (true) {
        switch (m_config.mode) {
        case manuel:
            modeManuel();
            break;
        case semiAuto:
            modeSemiAuto();
            break;
        case automatique:
            // TODO
            break;
        }
        delay(10);
    }
}

// renvoie true ssi obstacle sur la trajectoire (ie devant quand on avance)
bool CRoblind::modeManuel()
{
   position p;

   m_vitesse=m_canne->m_capteurForce->calculVitesse();
   cout << "JOYSTICK X : ";
   p = m_canne->m_joystick->getPosition();
   cout << m_canne->m_joystick->getX() << " - " << m_canne->m_joystick->getY() << endl;


   if ( p == 0 && m_etat != avance && !detecterObst()) // avancer
   {
        m_etat = avance;
        nouvelOrdre = true;
   }

    if ( p == 1 && m_etat != recule ) // reculer
    {
        m_etat = recule;
        nouvelOrdre = true;
    }

    if (p == 2 && m_etat != gogauche) //aller à gauche 90°
    {
        m_etat = gogauche;
        nouvelOrdre = true;
//        m_etat = arret;
    }

    if (p == 3 && m_etat != godroite) //aller a droite 90°
    {
        m_etat = godroite;
        nouvelOrdre = true;
//        m_etat = arret;
    }

    if (p == 6 && m_etat != anglegauche) //angle gauche 45°
    {
        m_etat = anglegauche;
        nouvelOrdre = true;
//        m_etat = arret;
    }

    if (p == 5 && m_etat != angledroit) //angle droit 45°
    {
        m_etat = angledroit;
        nouvelOrdre = true;
//        m_etat = arret;
    }

    if (m_canne->m_joystick->isPressed()) //stop
    {
        m_etat = arret;
        m_baseRoulante->stop();
    }

    if (m_etat == avance && detecterObst()) {
        m_etat = arret;
        m_baseRoulante->stop();
        return true;
    }

    return false;
}

bool CRoblind::modeSemiAuto()
{
    if (modeManuel() && !enEvitement) {
        // Manoeuvre d'evitement
        enEvitement = true;
         // TODO
    }

    return true;
}


bool CRoblind::modeAuto(int i)
{
    bool check=false;
    double distance=0;
    double angle=0;

        m_gps->posDestination.latitude=m_gps->latitudeXML[i];     
        m_gps->posDestination.longitude=m_gps->longitudeXML[i];
        m_gps->getPositionActuel();
        distance=m_gps->getDistance()*1000;
        angle=m_gps->getAngle();
        
        m_baseRoulante->tourner(km1,angle);  
        m_baseRoulante->allerEn(distance,avancer,km1,m);
        

    

return check;

}




bool CRoblind::detecterObst()
{
    bool retour=false;

    if(digitalRead(m_config.numPinIR) == 0)
    {
//         m_etat = arret;
//         m_baseRoulante->m_status=stope;
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
