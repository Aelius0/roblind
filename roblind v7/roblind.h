#ifndef ROBLIND_H
#define ROBLIND_H
#include "canne.h"
#include "baseroulante.h"
#include "Kinect.h"
#include "lidarLite.h"
#include "CcapteurIR.h"
#include "AX13.h"
#include "cgps.h"
#include "Grille.h"
#include "pthread.h"
//#include "AStar.h"

enum modeUtilisation
{
    manuel=0,automatique,semiAuto
};
enum etatRoblind
{
    avance=0,recule,arret,angledroit , anglegauche , gogauche , godroite
};
struct configRoblind
{
    modeUtilisation mode;
    int numPinIR;
    int m_pinServo1;
    int m_pinServo2;
    configCanne confCanne;
    configAX13 confAX13;
    configBaseRoulante confBaseRoulante;

};

class CRoblind
{
private:
    bool enEvitement;
    static pthread_mutex_t mut;
public:

    CRoblind(configRoblind config);
    configRoblind m_config;
    CCanne *m_canne;
    Cbaseroulante *m_baseRoulante;
    Kinect* m_Kinect;
    lidarLite *m_rightLidar;
    CapteurIR *m_capteurIR;
    AX13 *m_pinServo;
    cgps *m_gps;
    Grille* m_grille;

    etatRoblind m_etat;
    vitesse m_vitesse;
    bool nouvelOrdre;

    static void* executerMouvement(void*);
    void boucleMouvement();
    bool modeManuel();
    bool captPresence();
    bool modeAuto(int i);
    bool modeSemiAuto();
    bool detecterObst();

};


#endif // ROBLIND_H
