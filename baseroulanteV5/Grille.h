#ifndef GRILLE_H
#define GRILLE_H
#include <iostream>
#include <cmath>
#include <iomanip>


enum nature_sol {
    RAS= 0,
    OBSTACLE=1,
    MUR=2,
    CHAUSSEE=3
};

struct incorrectCoordonnees{};

class Grille{

	/* COMMENTAIRES D'UTILISATION :
	 * LA GRILLE EST ORGANISEE SELON LE FORMAT
	 * GRILLE[ABSCISSE][ORDONNEE]
	 * LES COORDONNEES D'UNE CASE S'IDENTIFIENT AVEC LES COORDONNES
	 * DE SON COIN INFERIEUR GAUCHE QUAND ON SE REPLACE DANS LE REPERAGE
	 * CONTINU DES POSITIONS SUR LA GRILLE.
	 * */

private:
    double resolution_; // en m
    int taille_;
    double tailleGrille_;
 
public:
    int tailleHitBox;
    char** grille_;
    Grille(double,int);
    ~Grille();
    void afficher();
    void translaterRepereGrille(int translationx, int translationy);
    void set(int i, int j, nature_sol type);
    int get(int i, int j);
    inline double getResolution(){return resolution_;}
    inline int getTaille(){return taille_;}
    inline double getTailleGrille(){return tailleGrille_;}
	void marquerObstacles(int x, int y);
};

#endif

