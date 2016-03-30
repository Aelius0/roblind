#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

class Grille{

private:
	double resolutionx_; // en m
	double resolutiony_; // en m
	int taillex_;
	int tailley_;
	int** grille_;
public:
	Grille(double resolutionx,double resolutiony,int taillex,int tailley){

		cout << "Construction de la grille.\n";

		/*CREATION DE LA GRILLE AVEC LES PARAMETRE RENTRES
		 * X = DIMENSION DES COLONNES
		 * Y = DIMENSION DES LIGNES
		 * */
		taillex_=taillex;
		tailley_=tailley;
		grille_ = new int*[taillex_];
		for(int i=0;i<taillex_;i++){
			grille_[i]=new int[tailley_];
		}
		resolutionx_=resolutionx;
		resolutiony_=resolutiony;

		/*INITIALISATION DE LA GRILLE*/
		for(int i=0;i<taillex_;i++){
			for(int j=0;j<tailley_;j++){
				grille_[i][j]=RAS;
			}
		}

		cout << "La grille est construite.\n";
	}

	Grille(int resolution, int taille){

		cout << "Construction de la grille.\n";

		/*CREATION DE LA GRILLE AVEC LES PARAMETRE RENTRES*/
		taillex_=taille;
		tailley_=taille;
		grille_ = new int*[taillex_];
		for(int i=0;i<taillex_;i++){
			grille_[i]=new int[tailley_];
		}
		resolutionx_=resolution;
		resolutiony_=resolution;

		/*INITIALISATION DE LA GRILLE*/
		for(int i=0;i<taillex_;i++){
			for(int j=0;j<tailley_;j++){
				grille_[i][j]=RAS;
			}
		}

		cout << "La grille est construite.\n";

	}

	~Grille(){
		cout << "Destruction de la grille.\n";
		for(int i=0;i<taillex_;i++){
			delete grille_[i];
		}
		delete grille_;
	}

	void afficher(){

		cout << "Affichage de la grille.\n";

        for(int j=tailley_-1;j>=0;--j){
            for(int i=0;i<taillex_;--i){
				cout << setw(3) << grille_[i][j];
			}
			cout << "\n";
		}

		cout << "Fin de l'affichage de la grille.\n";

	}

	void translaterRepereGrille(int translationx, int translationy){

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
			cout << "Etat = " << etat << "\n";

		/* ON PEUT ALORS TRANSLATER LE TABLEAU
		 * SELON LES PARAMETRES CALCULES
		 * */

		switch(etat){
		case 1: cout << "Etat 1\n";
			for(int i=0;i<taillex_;i++){
					for(int j=tailley_-1;j>=0;j--){
						if(i-translationx>=0
						&& i-translationx<taillex_
						&& j-translationy>=0
						&& j-translationy<tailley_){
							grille_[i][j]=grille_[i-translationx][j-translationy];
						}
						else{grille_[i][j]=RAS;}
					}
				}
				break;
		case 2: cout << "Etat 2\n";
			for(int i=taillex_-1;i>=0;i--){
					for(int j=tailley_-1;j>=0;j--){
						if(i-translationx>=0
						&& i-translationx<taillex_
						&& j-translationy>=0
						&& j-translationy<tailley_){
							grille_[i][j]=grille_[i-translationx][j-translationy];
						}
						else{grille_[i][j]=RAS;}
					}
				}
				break;
		case 3: cout << "Etat 3\n";
			for(int i=0;i<taillex_;i++){
					for(int j=0;j<tailley_;j++){
						if(i-translationx>=0
						&& i-translationx<taillex_
						&& j-translationy>=0
						&& j-translationy<tailley_){
							grille_[i][j]=grille_[i-translationx][j-translationy];
						}
						else{grille_[i][j]=RAS;}
					}
				}
				break;
		case 4: cout << "Etat 4\n";
			for(int i=taillex_-1;i>=0;i--){
					for(int j=0;j<tailley_;j++){
						if(i-translationx>=0
						&& i-translationx<taillex_
						&& j-translationy>=0
						&& j-translationy<tailley_){
							grille_[i][j]=grille_[i-translationx][j-translationy];
						}
						else{grille_[i][j]=RAS;}
					}
				}
				break;
		default: cout << "Probleme\n";
		}

	}

	void set(int i, int j, nature_sol type){
		try{
			if(i>=0 && i<taillex_ && j>=0 && j<tailley_){
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

	int get(int i, int j){
		int result;

		try{
			if(i>=0 && i<taillex_ && j>=0 && j<tailley_){
				result = grille_[i][j];
			}
			else{
				throw incorrectCoordonnees();
			}

			return result;
		}
		catch(incorrectCoordonnees& e){
			cout << "Les coordonnees rentrées sont incorrectes.\n";
		}

	}

	double getResolutionX(){return resolutionx_;}

	double getResolutionY(){return resolutiony_;}

	int getTailleX(){return taillex_;}

	int getTailleY(){return tailley_;}

};
