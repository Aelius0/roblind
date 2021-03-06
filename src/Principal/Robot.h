﻿
#ifndef __Robot_25af486e_96d7_45d7_b476_038f6f2f8d75_H_INCLUDED
#define __Robot_25af486e_96d7_45d7_b476_038f6f2f8d75_H_INCLUDED


//standard include
#include <iostream>
#include <pthread.h>
#include <queue>
#include <cmath>
//#include <wiringPi.h>
using namespace std;
//#include "/home/mecatro/Raspberry/usr/include/wiringPi.h"
//#include "../Canne/Joystick.h"
#include "../Canne/Bouton_arret.h"
//#include "../Canne/Capteur_effort.h"
//#include "../Canne/Capteur_presence.h"
#include "../Canne/Buzzer.h"
//#include "../Canne/Vibreur.h"
#include "../Capteurs/Kinect.h"
#include "../Principal/GPS.h"
#include "../Principal/Batterie.h"
//#include "../Capteurs/Laser.h"
#include "../Principal/Bouton_arret_urgence.h"
#include "../Principal/Bouton_mode_guidage.h"


#define DIMENSION 500
#define RESOLUTION 5.0
#define CM_TO_PAS_CODEUR 1

// PIN NUMBERS
#define NUM_PIN_BASE_ROULANTE 27
#define NUM_PIN_VIBREUR 0


class Robot
{
 public:
    /*COMPOSANTS DE LA CANNE*/
    CCanne* m_canne;

    /*BASE ROULANTE*/
    Cbaseroulante* baseRoulante;
    
    /*CAPTEURS DE LA BASE MOBILE*/
    Kinect* kinect;
    GPS* gps;
    Batterie* batterie;
    lidarLite* m_lidarLite;
    double hauteurLaser = 10; // TODO : fix
    double hauteurKinect = 15; // TODO : fix
    
    /*BOUTONS D'ARRET ET DE SELECTION DE MODE*/
    Bouton_arret_urgence* bouton_arret_urgence;
    Bouton_mode_guidage* bouton_mode_guidage;
    bool en_evitement;
    
    /*ATTRIBUTS DE POSITION ET DE DEPLACEMENT*/
    double x; // abscisse
    double y; // ordonnee
    double o; //orientation (EN RADIANS)
    double v; // vitesse du roblind dans la direction courante (orientation)
    //int caseX; // abscisse de la case correspondante dans la grille AJOUTE
    //int caseY; // ordonnee de la case correspondante dans la grille AJOUTE
    // 0: vide  1: obstacle  2: trottoir  3: mur
    Grille* grille; // grille des obstacles
    
    /*PARAMETRES LIES A LA TRANSLATION DE LA GRILLE*/
    double bordureSortie; // bordure a partir de laquelle translater la grille AJOUTE
    double bordureEntree; // bordure de reapparition du robot apres translation AJOUTE
    int caseInitialeXY; // abscisse et ordonnee de la case de reapparition AJOUTE
    double positionInitialeXY; // abscisse et ordonnee initiale du robot AJOUTE
    
    /*TRAJECTOIRE CALCULEE*/
    int* chemin_evitement[];

    /*CONSTRUCTEUR ET DESTRUCTEUR*/
    Robot(int taille = DIMENSION, double resolution = RESOLUTION);
    ~Robot();

    /*METHODES DE DEPLACEMENT*/
    void avancerRobot(double); // avec une distance absolue AJOUTE
    void translaterGrille(); // translate la grille quand le robot atteint la bordure de sortie AJOUTE
    bool calculer_chemin(double direction = getDirection(), double dist = 150);
    void actualiserLaser(double,double); //distance en metres, angle en radian
    void translaterGrille();
    bool obstacleDevant(int dist = 10);
    double getDirection(double dist = 150);
    bool stop();
    
    /*METHODE PRINCIPALE*/
    void main();
    bool obstacleDevant();
    bool calculerChemin(); // ??

}; //end class Robot

extern Robot robot;
extern queue< vector<int> > path;
extern int speed = 2;
extern int rotationSpeed = 2;
extern bool modeGPS;

#endif // __Robot_25af486e_96d7_45d7_b476_038f6f2f8d75_H_INCLUDED
