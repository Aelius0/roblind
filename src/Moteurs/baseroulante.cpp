#include "baseroulante.h"

Cbaseroulante::Cbaseroulante(const char *portName , int numPin)
{
    m_kangaroo = new Ckangaroo(portName); 
    m_relay = new CRelay(numPin);
    m_kangaroo->init();


}

void Cbaseroulante::getPosition(positionBaseRoulante &posBR)
{
    posBR = m_posBR;

}

int Cbaseroulante::getDistanceParcourue(int &distance,unite u)
{
    int erreur = 0;
    int posCodeur = 0;
    erreur=m_kangaroo->getPosition(drive, posCodeur);
    distance = (int)(posCodeur / (int)u);

    return erreur;

}

int Cbaseroulante::getVitesse(int &speed,unite u)
{
    int erreur = 0;
    int posCodeur = 0;
    erreur=m_kangaroo->getSpeed(drive, posCodeur);
    speed = (int)(posCodeur / (int)u);

    return erreur;

}



int Cbaseroulante::getAngleTourne(int &angle)
{
    int erreur = 0;
    int posCodeur = 0;
    erreur= m_kangaroo->getPosition(turn, posCodeur);
    angle = (int)(posCodeur/ (int)degre);

    return erreur;

}

void Cbaseroulante::stop()
{
    m_status = stope;
    m_kangaroo->start(drive);

}

int Cbaseroulante::recallerAngle(int alpha)
{
    int beta = alpha;
    if (beta <= -180)
    {
        beta += 360;
    }
    if(beta > 180)
    {
        beta -= 360;
    }

    return beta;


}

etatBR Cbaseroulante::setVitesse (vitesse v, sens s  )
{
    int erreur = 0;
    int vitesseReelle = 0;
    int vitesseReelle_tm1=0;

    int delta =0;


    m_status=notDefined;
    v=(vitesse)(v*(int)s);
    m_kangaroo->setVitesse(v);

    do
     {
       // vitesseReelle_tm1 = vitesseReelle;
        //erreur = getVitesse(vitesseReelle,u);
         if(erreur == 0xE3)
         {
             //vitesseReelle = vitesseReelle_tm1;

             m_status = bloque;
             m_relay->TurnOn();
             usleep(1000);
             m_relay->TurnOff();
             usleep(1000);
             m_kangaroo->init();

         }
       /*  else
         {

             /*delta = abs(v - vitesseReelle);
             if (delta <= 50)
             {
                 m_status = arrive;
                 usleep(500);
                 erreur = getVitesse(vitesseReelle,u);
                 m_kangaroo->init();

             }



         } */

     }while (m_status != arrive && m_status != bloque && m_status != stope);


    return m_status;






}

/*etatBR Cbaseroulante::allerEn (int distance, sens s , int speed, unite u)
{
    int erreur = 0;
    int distanceReelle = 0;
    int distanceReelle_tm1=0;

    int delta =0;

    m_status=notDefined;
    v=(vitesse)(v*(int)s);

    distance*=s;
    m_kangaroo->allerEn(distance,speed, u);

    do
     {
         distanceReelle_tm1 = distanceReelle;
         erreur = getDistanceParcourue(distanceReelle,u);
         if(erreur == 0xE3)
         {
             distanceReelle = distanceReelle_tm1;
             m_status = bloque;
             m_relay->TurnOn();
             usleep(1000);
             m_relay->TurnOff();
             usleep(1000);
             m_kangaroo->init();

         }
         else
         {
            delta = abs(distance - distanceReelle);
            if (delta <= 5)
            {
                m_status = arrive;
                usleep(500);
                erreur = getDistanceParcourue(distanceReelle,u);
                m_kangaroo->init();

            }

         }

     }while (m_status != arrive && m_status != bloque && m_status != stope);


    return m_status;

}*/

etatBR Cbaseroulante::tourner(vitesse v, int alphaConsigne)
{
    int erreur = 0;
    int alphaReel = 0;
    int alphaReel_tm1 = 0;
    int delta = 0;

    m_status = notDefined;
    m_kangaroo->tourner(v,alphaConsigne);
    do
    {
        alphaReel_tm1 = alphaReel;
        erreur = getAngleTourne(alphaReel);
        if(erreur == 0xE3)
        {
            alphaReel = alphaReel_tm1;
            m_status = bloque;
            m_relay->TurnOn();
            usleep(1000);
            m_relay->TurnOff();
            usleep(1000);
            m_kangaroo->init();
        }
        else
        {
            delta = abs(alphaConsigne - alphaReel);
            if(delta <= 3)
            {
                m_status = arrive;
                usleep(500);
                getAngleTourne(alphaReel);
                m_kangaroo->init();
            }

        }


    }while (m_status != arrive && m_status != bloque && m_status != stope);

    return m_status;

}


