
#ifndef __Robot_25af486e_96d7_45d7_b476_038f6f2f8d75_H_INCLUDED
#define __Robot_25af486e_96d7_45d7_b476_038f6f2f8d75_H_INCLUDED

/*
 * File Type: Class header
 * Class: Robot
 */

//owner package header file
#include "Principal.h"
//automatic includes (friends, associated classes, etc)
#include "../Canne/Joystick.h"
#include "../Canne/Bouton_arret.h"
#include "../Canne/Capteur_effort.h"
#include "../Canne/Capteur_presence.h"
#include "../Canne/Buzzer.h"
#include "../Canne/Vibreur.h"
#include "../Capteurs/Kinect.h"
#include "../Moteurs/Moteur.h"
#include "../Principal/GPS.h"
#include "../Principal/Batterie.h"
#include "../Capteurs/Laser.h"
#include "../Principal/Bouton_arret_urgence.h"
#include "../Principal/Bouton_mode_guidage.h"
#include <iostream>
using namespace std;
#include "/home/mecatro/Raspberry/usr/include/wiringPi.h"


namespace Principal {


#define DIMENSION 500


class Robot
{
 public:
    double resolution = 5.0; //résolution en centimètres
    bool en_evitement;
    Canne::Joystick* joystick;
    Canne::Bouton_arret* bouton_arret;
    Canne::Capteur_effort* capteur_effort;
    Canne::Capteur_presence* capteur_presence;
    Canne::Buzzer* buzzer;
    Canne::Vibreur* vibreur;
    Capteurs::Kinect* kinect;
    Moteurs::Moteur* moteur;
    GPS* gps;
    Batterie* batterie;
    Capteurs::Laser* laser_gauche;
    Capteurs::Laser* laser_droit;
    Bouton_arret_urgence* bouton_arret_urgence;
    Bouton_mode_guidage* bouton_mode_guidage;
	
 private:
    int grille_espace[DIMENSION][DIMENSION];
    double x_local;
    double y_local;
    double o_local;
    int* chemin_evitement[];
	
	
void main();
void marquer_bloque(int x, int y);
void marquer_libre(int x, int y);
bool calculer_chemin(double direction);


}; //end class Robot

} // namespace Principal
#endif // __Robot_25af486e_96d7_45d7_b476_038f6f2f8d75_H_INCLUDED
