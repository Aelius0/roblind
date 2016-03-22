
#ifndef __Moteurs_8e1bc01c_abbd_488d_9630_5c9978d629bb_H_INCLUDED
#define __Moteurs_8e1bc01c_abbd_488d_9630_5c9978d629bb_H_INCLUDED


int* position = new int;
int lastCmd = -1;
int lastCmdType = -1;
int err = 0;

void *ThreadMoteur (void *); //thread de lecture et d'ecriture de la carte de commande des moteurs


#endif // __Moteurs_8e1bc01c_abbd_488d_9630_5c9978d629bb_H_INCLUDED
