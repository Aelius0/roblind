
#ifndef __Robot_25af486e_96d7_45d7_b476_038f6f2f8d75_H_INCLUDED
#define __Robot_25af486e_96d7_45d7_b476_038f6f2f8d75_H_INCLUDED

/*
 * File Type: Class header
 * Class: Robot
 */

//owner package header file
#include "Principal/Principal.hxx"
//automatic includes (friends, associated classes, etc)
#include "Canne/Joystick.hxx"
#include "Canne/Bouton_arret.hxx"
#include "Canne/Capteur_effort.hxx"
#include "Canne/Capteur_presence.hxx"
#include "Canne/Buzzer.hxx"
#include "Canne/Vibreur.hxx"
#include "Capteurs/Kinect.hxx"
#include "Moteurs/Moteur.hxx"
#include "Principal/GPS.hxx"
#include "Principal/Batterie.hxx"
#include "Capteurs/Laser.hxx"
#include "Principal/Bouton_arret_urgence.hxx"
#include "Principal/Bouton_mode_guidage.hxx"



namespace Principal {




class Robot
{
 public:
    double resolution;
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
void run();
void marquer_bloque(int x, int y);
void marquer_libre(int x, int y);
void calculer_chemin(double direction);

 private:
    void* grille_espace;
    double x_local;
    double y_local;
    double o_local;
    void* chemin_evitement;

}; //end class Robot

} // namespace Principal
#endif // __Robot_25af486e_96d7_45d7_b476_038f6f2f8d75_H_INCLUDED
