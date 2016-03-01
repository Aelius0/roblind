
#ifndef __Robot_25af486e_96d7_45d7_b476_038f6f2f8d75_H_INCLUDED
#define __Robot_25af486e_96d7_45d7_b476_038f6f2f8d75_H_INCLUDED

/*
 * File Type: Class header
 * Class: Robot
 */

//standard include
#include <iostream>
#include <pthread.h>
using namespace std;
//#include "/home/mecatro/Raspberry/usr/include/wiringPi.h"
#include "../Moteurs/Moteur.h"
#include "../Canne/Joystick.h"
#include "../Canne/Bouton_arret.h"
#include "../Canne/Capteur_effort.h"
#include "../Canne/Capteur_presence.h"
#include "../Canne/Buzzer.h"
#include "../Canne/Vibreur.h"
#include "../Capteurs/Kinect.h"
#include "../Principal/GPS.h"
#include "../Principal/Batterie.h"
#include "../Capteurs/Laser.h"
#include "../Principal/Bouton_arret_urgence.h"
#include "../Principal/Bouton_mode_guidage.h"


#define DIMENSION 500
#define RESOLUTION 5.0


class Robot
{
 public:
    bool en_evitement;
    Canne::Joystick* joystick;
    Canne::Bouton_arret* bouton_arret;
    Canne::Capteur_effort* capteur_effort;
    Canne::Capteur_presence* capteur_presence;
    Canne::Buzzer* buzzer;
    Canne::Vibreur* vibreur;
    Kinect* kinect;
    Moteur* moteur;
    GPS* gps;
    Batterie* batterie;
    Laser* laser_gauche;
    Laser* laser_droit;
    Bouton_arret_urgence* bouton_arret_urgence;
    Bouton_mode_guidage* bouton_mode_guidage;
    double x;
    double y;
    double o; //orientation
    int* chemin_evitement[];
    // 0: vide  1: obstacle  2: trottoir  3: mur
    int grille_espace[DIMENSION][DIMENSION];
	
	
void main();
void marquer_bloque(int x, int y);
void marquer_libre(int x, int y);
bool calculer_chemin(double direction);


}; //end class Robot

extern Robot robot;
extern vector< vector<int> > path;
#endif // __Robot_25af486e_96d7_45d7_b476_038f6f2f8d75_H_INCLUDED
