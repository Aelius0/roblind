
#ifndef __Moteurs_8e1bc01c_abbd_488d_9630_5c9978d629bb_H_INCLUDED
#define __Moteurs_8e1bc01c_abbd_488d_9630_5c9978d629bb_H_INCLUDED

#include "../Principal/Robot.h"
#include "baseroulante.h"

positionBaseRoulante* posBR(0,0,0);
int lastCmd = -1;
int lastCmdType = -1;
bool noWay = false;

extern Cbaseroulante baseRoulante;
void *ThreadMoteur (void *); //thread de lecture et d'ecriture de la carte de commande des moteurs

extern bool active;


#endif // __Moteurs_8e1bc01c_abbd_488d_9630_5c9978d629bb_H_INCLUDED
