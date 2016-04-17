/*INCLUSION DES BIBLIOTHEQUES*/
#include <iostream>
#include <cmath>
#include <iomanip>
/*INCLUSION DES HEADERS*/
#include "Grille.h"
/*NAMESPACES*/
using namespace std;


/*CONSTRUCTEUR ET DESTRUCTEUR*/
	Grille::Grille(int nombreCases, double resolutionCase){

		cout << "Construction de la grille.\n";

		/*CREATION DE LA GRILLE AVEC LES PARAMETRE RENTRES
		 * X = ABSCISSE = INDICE DE LA COLONNE
		 * Y = ORDONNEE = INDICE DE LA LIGNE
		 * */
		this->dimension=nombreCases;
		grille = new int*[dimension];
		for(int i=0;i<dimension;i++){
			grille[i]=new int[dimension];
		}
		this->resolution=resolutionCase;
		taille = dimension * resolution ;

		/*INITIALISATION DE LA GRILLE*/
		for(int i=0;i<dimension;i++){
			for(int j=0;j<dimension;j++){
				grille[i][j]=RAS;
			}
		}

		cout << "La grille est construite.\n";
	}
	Grille::~Grille(){
		cout << "Destruction de la grille.\n";
		for(int i=0;i<dimension;i++){
			delete grille[i];
		}
		delete grille;
	}

/*GETTERS ET SETTERS*/
	int Grille::get(int i, int j){
		int result;
		try{
			if(i>=0 && i<dimension && j>=0 && j<dimension){
				result = grille[i][j];
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
	void Grille::set(int i, int j, nature_sol type){

		try{
			if(i>=0 && i<dimension && j>=0 && j<dimension){
				grille[i][j]=type;
			}
			else{
				throw incorrectCoordonnees();
			}
		}
		catch(incorrectCoordonnees& e){
			cout << "Les coordonnees rentrées sont incorrectes.\n";
		}
	}

/*METHODES GENERALES*/
	void Grille::afficher(){

		cout << "-----------------------\n";
		cout << "AFFICHAGE DE LA GRILLE.\n";

        for(int j=dimension-1;j>=0;--j){
        	cout << setw(4) << j << " |"; // Affichage de l'ordonnee
            for(int i=0;i<dimension;++i){
				cout << setw(3) << grille[i][j];
			}
			cout << "\n";
		}

        /*AFFICHAGE DE LA LIGNE DES ABSCISSES*/
        cout << "------";
        for(int i=0;i<dimension;++i){
        	cout << "---";
        }
        cout << "\n Y/X |";
        for(int i=0;i<dimension;++i){
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
			for(int i=0;i<dimension;i++){
					for(int j=dimension-1;j>=0;j--){
						if(i-translationx>=0
						&& i-translationx<dimension
						&& j-translationy>=0
						&& j-translationy<dimension){
							grille[i][j]=grille[i-translationx][j-translationy];
						}
						else{grille[i][j]=RAS;}
					}
				}
				break;
		case 2: //cout << "Etat 2\n";
			for(int i=dimension-1;i>=0;i--){
					for(int j=dimension-1;j>=0;j--){
						if(i-translationx>=0
						&& i-translationx<dimension
						&& j-translationy>=0
						&& j-translationy<dimension){
							grille[i][j]=grille[i-translationx][j-translationy];
						}
						else{grille[i][j]=RAS;}
					}
				}
				break;
		case 3: //cout << "Etat 3\n";
			for(int i=0;i<dimension;i++){
					for(int j=0;j<dimension;j++){
						if(i-translationx>=0
						&& i-translationx<dimension
						&& j-translationy>=0
						&& j-translationy<dimension){
							grille[i][j]=grille[i-translationx][j-translationy];
						}
						else{grille[i][j]=RAS;}
					}
				}
				break;
		case 4: //cout << "Etat 4\n";
			for(int i=dimension-1;i>=0;i--){
					for(int j=0;j<dimension;j++){
						if(i-translationx>=0
						&& i-translationx<dimension
						&& j-translationy>=0
						&& j-translationy<dimension){
							grille[i][j]=grille[i-translationx][j-translationy];
						}
						else{grille[i][j]=RAS;}
					}
				}
				break;
		default: cout << "Probleme\n";
		}

	}








