#ifndef GRILLE_H
#define GRILLE_H

/*INCLUSION DES BIBLIOTHEQUES*/
#include <iostream>
#include <cmath>
#include <iomanip>
/*INCLUSION DES HEADERS*/


/*CARACTERISATION DU SOL*/
enum nature_sol : int {
    RAS= 0,
    OBSTACLE=1,
    MUR=2,
    CHAUSSEE=3
};

/*STRUCTURES D'ERREURS*/
struct incorrectCoordonnees{};

class Grille{

	/* COMMENTAIRES D'UTILISATION :
	 * LA GRILLE EST ORGANISEE SELON LE FORMAT
	 * GRILLE[ABSCISSE][ORDONNEE]
	 * LA CASE X EST LA PARTIE ENTIERE DE LA POSITION X ET IDEM POUR Y
	 * POUR OBTENIR LA VERITABLE POSITION, IL FAUT MULTIPLIER PAR LA TAILLE DE LA GRILLE
	 * */

private:
	/*PARAMETRES DE LA GRILLE*/
    int dimension;				// Ce sont les dimensions de la grille (carree)
    double resolution; 			// C'est la taille d'une case.
    double taille;		// C'est la taille totale de la grille.
    /*GRILLE*/
    int** grille;
public:
    /*CONSTRUCTEUR ET DESTRUCTEUR*/
    Grille(int nombreCases = 20, double resolutionCase = 1.0);			// Constructeur
    ~Grille();															// Destructeur
    /*GETTERS ET SETTERS*/
    void set(int i, int j, nature_sol type);				// Modifier la nature d'une case de la grille.
    int get(int i, int j);									// Obtenir la nature d'une case de la grille.
    inline double getResolution(){return resolution;}		// Obtenir la resolution de la grille.
    inline int getDimension(){return dimension;}				// Obtenir les dimensions de la grille.
    inline double getTaille(){return taille;}	// Obtenir la taille totale de la grille.
    /*METHODES GENERALES*/
    void afficher();														// Afficher la grille entière.
    void translaterRepereGrille(int translationx, int translationy);		// Changer l'origine de la grille.

};

#endif
