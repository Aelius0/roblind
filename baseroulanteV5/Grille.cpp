#include <iostream>
#include <cmath>
#include <iomanip>
#include "Grille.h"
using namespace std;


	Grille::Grille(double resolution,int taille){

		cout << "Construction de la grille.\n";

		/*CREATION DE LA GRILLE AVEC LES PARAMETRE RENTRES
		 * X = ABSCISSE = INDICE DE LA COLONNE
		 * Y = ORDONNEE = INDICE DE LA LIGNE
		 * */
		 
		taille_=taille;
		
		tailleHitBox = 50/resolution+1;
		
        grille_ = new char*[taille_];
		for(int i=0;i<taille_;i++){
            grille_[i]=new char[taille_];
		}
		resolution_=resolution;
		tailleGrille_ = taille_ * resolution_ ;

		/*INITIALISATION DE LA GRILLE*/
		for(int i=0;i<taille_;i++){
			for(int j=0;j<taille_;j++){
				grille_[i][j]=RAS;
			}
		}

		cout << "La grille est construite.\n";
	}

	Grille::~Grille(){
		cout << "Destruction de la grille.\n";
		for(int i=0;i<taille_;i++){
			delete grille_[i];
		}
		delete grille_;
	}

	void Grille::afficher(){

		cout << "-----------------------\n";
		cout << "AFFICHAGE DE LA GRILLE.\n";

        for(int j=taille_-1;j>=0;--j){
        	cout << setw(4) << j << " |"; // Affichage de l'ordonnee
            for(int i=0;i<taille_;++i){
				cout << setw(3) << grille_[i][j];
			}
			cout << "\n";
		}

        /*AFFICHAGE DE LA LIGNE DES ABSCISSES*/
        cout << "------";
        for(int i=0;i<taille_;++i){
        	cout << "---";
        }
        cout << "\n Y/X |";
        for(int i=0;i<taille_;++i){
        	cout << setw(3) << i;
        }

		cout << "\nFIN DE L'AFFICHAGE DE LA GRILLE.\n";

	}

	void Grille::translaterRepereGrille(int translationx, int translationy){

		/* IDEE : ON PARCOURT LE TABLEAU ET A CHAQUE CASE EST ATTRIBUEE SA VALEUR FINALE
		 * SI LA CASE DOIT PREALABLEMENT ATTRIBUER UNE VALEUR, CE DEVRA ETRE FAIT AVANT.
		 * LE TEST CI-DESSOUS DETERMINE LE SENS DE PARCOURS
		 * */

			int etat;

			/*SENS DE PARCOURS SELON X*/
			if(translationx>=0 && translationy>=0){etat=2;}
			else if(translationx>=0 && translationy<0){etat=4;}
			else if(translationx<0 && translationy>=0){etat=1;}
			else if(translationx<0 && translationy<0){etat=3;}
			//cout << "Etat = " << etat << "\n";

		/* ON PEUT ALORS TRANSLATER LE TABLEAU
		 * SELON LES PARAMETRES CALCULES
		 * */

		switch(etat){
		case 1: //cout << "Etat 1\n";
			for(int i=0;i<taille_;i++){
					for(int j=taille_-1;j>=0;j--){
						if(i-translationx>=0
						&& i-translationx<taille_
						&& j-translationy>=0
						&& j-translationy<taille_){
							grille_[i][j]=grille_[i-translationx][j-translationy];
						}
						else{grille_[i][j]=RAS;}
					}
				}
				break;
		case 2: //cout << "Etat 2\n";
			for(int i=taille_-1;i>=0;i--){
					for(int j=taille_-1;j>=0;j--){
						if(i-translationx>=0
						&& i-translationx<taille_
						&& j-translationy>=0
						&& j-translationy<taille_){
							grille_[i][j]=grille_[i-translationx][j-translationy];
						}
						else{grille_[i][j]=RAS;}
					}
				}
				break;
		case 3: //cout << "Etat 3\n";
			for(int i=0;i<taille_;i++){
					for(int j=0;j<taille_;j++){
						if(i-translationx>=0
						&& i-translationx<taille_
						&& j-translationy>=0
						&& j-translationy<taille_){
							grille_[i][j]=grille_[i-translationx][j-translationy];
						}
						else{grille_[i][j]=RAS;}
					}
				}
				break;
		case 4: //cout << "Etat 4\n";
			for(int i=taille_-1;i>=0;i--){
					for(int j=0;j<taille_;j++){
						if(i-translationx>=0
						&& i-translationx<taille_
						&& j-translationy>=0
						&& j-translationy<taille_){
							grille_[i][j]=grille_[i-translationx][j-translationy];
						}
						else{grille_[i][j]=RAS;}
					}
				}
				break;
		default: cout << "Probleme\n";
		}

	}

	void Grille::set(int i, int j, nature_sol type){

		try{
			if(i>=0 && i<taille_ && j>=0 && j<taille_){
				grille_[i][j]=type;
			}
			else{
				throw incorrectCoordonnees();
			}
		}
		catch(incorrectCoordonnees& e){
			cout << "Les coordonnees rentrées sont incorrectes.\n";
		}
	}

	int Grille::get(int i, int j){
		int result;
		try{
			if(i>=0 && i<taille_ && j>=0 && j<taille_){
				result = grille_[i][j];
			}
			else{
				throw incorrectCoordonnees();
			}


		}
		catch(incorrectCoordonnees& e){
			cout << "Les coordonnees rentrées sont incorrectes.\n";
		}
		return result;
	}
	
	void Grille::marquerObstacles(int x, int y){
		for (int i=x-tailleHitBox; i<x+tailleHitBox; i++){
			for (int j=y-tailleHitBox; j<y+tailleHitBox; j++){
                if (i>0 && i<taille_ && j>0 && j<taille_){
                    set(i, j, OBSTACLE);
				}
			}
		}
	}



