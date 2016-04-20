#ifndef ROBLIND_H
#define ROBLIND_H
#include "canne.h"
#include "baseroulante.h"
#include "lidarLite.h"
#include "CcapteurIR.h"

enum etatRoblind
{
    avance=0,recule,arret,angledroit , anglegauche , gogauche , godroite
};
struct configRoblind
{
    int numPinIR;
    configCanne confCanne;
    configBaseRoulante confBaseRoulante;

};

class CRoblind
{
public:
    CRoblind(configRoblind config);
    configRoblind m_config;
    CCanne *m_canne;
    Cbaseroulante *m_baseRoulante;
    lidarLite *m_lidarLite;
    CapteurIR *m_capteurIR;
    etatRoblind m_etat;
    vitesse m_vitesse;


    bool allerEn();
    bool captPresence();
    bool detecterObst();




};

#endif // ROBLIND_H
