#ifndef BASEROULANTE_H
#define BASEROULANTE_H
#include "kangaroo.h"
#include "relay.h"
#include <cmath>
#include <unistd.h>
#include <sys/time.h>
#include <ctime>

enum etatBR
{
    notDefined=0,arrive=1,bloque=2,stope=3
};

enum sens
{
    avancer=-1, reculer=1
};


struct positionBaseRoulante
{
    int x;
    int y;
    int alpha;
};

struct configBaseRoulante
{
    char numPort[100];
    int numPinRelai;
};

class Cbaseroulante
{
public:
    Cbaseroulante(const char *portName, int numPin);

    Ckangaroo *m_kangaroo;
    CRelay *m_relay;
    positionBaseRoulante m_posBR;
    etatBR m_status;

    void getPosition(positionBaseRoulante &posBR);
    int getDistanceParcourue(int &distance,unite u);
    int getVitesse(int &speed,unite u);
    int getAngleTourne(int &angle);
    void stop();
    etatBR allerEn (int distance, sens s , vitesse v, unite u);
    etatBR setVitesse (vitesse v, sens s);
    etatBR tourner (vitesse v,int alphaConsigne );
    int recallerAngle(int alpha);




};

#endif // BASEROULANTE_H
