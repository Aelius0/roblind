#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

enum nature_sol : int {
	RAS	= 0,
	OBSTACLE=1,
	MUR=2,
	CHAUSSEE=3
};

struct incorrectCoordonnees{};

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

	void translaterRepereUsuel(int translationx, int translationy){

		/* IDEE : ON PARCOURT LE TABLEAU ET A CHAQUE CASE EST ATTRIBUEE SA VALEUR FINALE
		 * SI LA CASE DOIT PREALABLEMENT ATTRIBUER UNE VALEUR, CE DEVRA ETRE FAIT AVANT.
		 * LE TEST CI-DESSOUS DETERMINE LE SENS DE PARCOURS
		 * */

			int etat;

			/*SENS DE PARCOURS SELON X*/
			if(translationx>=0 && translationy>=0){etat=1;}
			else if(translationx>=0 && translationy<0){etat=2;}
			else if(translationx<0 && translationy>=0){etat=3;}
			else if(translationx<0 && translationy<0){etat=4;}
			cout << "Etat = " << etat << "\n";

		/* ON PEUT ALORS TRANSLATER LE TABLEAU
		 * SELON LES PARAMETRES CALCULES
		 * */

		switch(etat){
		case 1: cout << "Etat 1\n";
			for(int i=0;i<taillex_;i++){
					for(int j=tailley_-1;j>=0;j--){
						if(i+translationx>=0
						&& i+translationx<taillex_
						&& j-translationy>=0
						&& j-translationy<tailley_){
							grille_[i][j]=grille_[i+translationx][j-translationy];
						}
						else{grille_[i][j]=RAS;}
					}
				}
				break;
		case 2: cout << "Etat 2\n";
			for(int i=taillex_-1;i>=0;i--){
					for(int j=tailley_-1;j>=0;j--){
						if(i+translationx>=0
						&& i+translationx<taillex_
						&& j-translationy>=0
						&& j-translationy<tailley_){
							grille_[i][j]=grille_[i+translationx][j-translationy];
						}
						else{grille_[i][j]=RAS;}
					}
				}
				break;
		case 3: cout << "Etat 3\n";
			for(int i=0;i<taillex_;i++){
					for(int j=0;j<tailley_;j++){
						if(i+translationx>=0
						&& i+translationx<taillex_
						&& j-translationy>=0
						&& j-translationy<tailley_){
							grille_[i][j]=grille_[i+translationx][j-translationy];
						}
						else{grille_[i][j]=RAS;}
					}
				}
				break;
		case 4: cout << "Etat 4\n";
			for(int i=taillex_-1;i>=0;i--){
					for(int j=0;j<tailley_;j++){
						if(i+translationx>=0
						&& i+translationx<taillex_
						&& j-translationy>=0
						&& j-translationy<tailley_){
							grille_[i][j]=grille_[i+translationx][j-translationy];
						}
						else{grille_[i][j]=RAS;}
					}
				}
				break;
		default: cout << "Probleme\n";
		}
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

/*
class Robot{

private:
	double positionX_,positionY_;
	int caseX_, caseY_;
	double positionXIni_,positionYIni_;
	int caseXIni_, caseYIni_;
	int bordureX_,bordureY_;
	double orientation_;
	double vitesse_;
	Grille* grille_;
	double hauteurLaser_;
public:
	Robot(int tailleX, int tailleY, double resolutionX, double resolutionY, int bordureX, int bordureY){

        cout << "Creation du robot.\n";

		grille_ = new Grille(resolutionX,resolutionY,tailleX,tailleY);

		orientation_=0; // Le robot est dirigé vers le haut de la grille
		vitesse_=0;
		bordureX_=bordureX;
		bordureY_=bordureY;

		caseXIni_ = floor(grille_->getTailleX()/2);
		caseYIni_ = floor(grille_->getTailleY()/2);
		caseX_=caseXIni_;
		caseY_=caseYIni_;
		positionXIni_=resolutionX*caseXIni_+resolutionX/2;
		positionYIni_=resolutionY*caseYIni_+resolutionY/2;	// Le robot commence au milieu de la grille
		positionX_=positionXIni_;
		positionY_=positionYIni_;

		hauteurLaser_ = 10;

		cout << "Fin de la creation du robot.\n";

	}

	~Robot(){
		delete grille_;
		cout << "Le robot est detruit.\n";
	}
/*
	double getPositionX(){return positionX_;}
	double getPositionY(){return positionY_;}
	double getPositionXIni(){return positionXIni_;}
	double getPositionYIni(){return positionYIni_;}
	double getBordureX(){return bordureX_;}
	double getBordureY(){return bordureY_;}
	double getCaseXIni(){return caseXIni_;}
	double getCaseYIni(){return caseYIni_;}
	double getCaseX(){return caseX_;}
	double getCaseY(){return caseY_;}
	double getVitesse(){return vitesse_;}
	double getOrientation(){return orientation_;}
	Grille* getGrille(){return grille_;}

	void setPositionX(double newPositionX){
		positionX_=newPositionX;
        caseX_= static_cast<int>(positionX_/(grille_->getResolutionX()));
	}
	void setPositionY(double newPositionY){
		positionY_=newPositionY;
        caseY_= static_cast<int>(positionX_/(grille_->getResolutionY()));
	}
	void setVitesse(double newVitesse){vitesse_=newVitesse;}
	void setOrientation(double newOrientation){orientation_=newOrientation;}

    void actualiserLaser(double inclinaison, double distance){

        double distanceTheorique = hauteurLaser_ / cos(inclinaison);

        double deltaTheorique = distance - distanceTheorique;

        double projectionLaterale = distance * sin(inclinaison);

            double positionObstacleXrel = - projectionLaterale * sin(orientation_);
            double positionObstacleYrel = - projectionLaterale * cos(orientation_);
            double positionObstacleXabs = positionX_+positionObstacleXrel;
            double positionObstacleYabs = positionY_+positionObstacleYrel;

            int caseObstacleX = floor(positionObstacleXabs/(grille_->getResolutionX()));
            int caseObstacleY = floor(positionObstacleYabs/(grille_->getResolutionY()));

        if(deltaTheorique>0){grille_->set(caseObstacleX,caseObstacleY,OBSTACLE);}
        else{grille_->set(caseObstacleX,caseObstacleY,CHAUSSEE);}

        }
*/
	};



int main(){

	/*CREATION DE LA GRILLE INITIALISEE*/
	/*
	Grille* essai = new Grille(10,10,10,10);
	essai->afficher();
	 */
	/*REMPLISSAGE DE LA GRILLE*/
	/*
	cout << "Remplissage de la grille.\n";
	for(int i=0;i<10;i+=2){
		for(int j=0;j<10;j+=2){
			essai->set(i,j,OBSTACLE);
			cout << "Grille[" << i << "][" << j << "] = " << essai->get(i,j) << "\n";
		}
	}
	essai->afficher();
	*/
	/*TRANSLATION DE LA GRILLE*/
	/*
	cout << "Translation de la grille.\n";
	essai->translaterRepereUsuel(1,1);
	essai->afficher();
	essai->translaterRepereGrille(1,-1);
	essai->afficher();
	*/

	/*CREATION DU ROBOT ET DE LA GRILLE*/
	Robot* robot = new Robot(10,10,5,5,1,1);
	robot->getGrille()->afficher();
	cout << "CaseX : " << robot->getCaseX() << "\n";
	cout << "CaseY : " << robot->getCaseY() << "\n";
	cout << "PositionX : " << robot->getPositionX() << "\n";
	cout << "PositionY : " << robot->getPositionY() << "\n";
	robot->setPositionX(7.6);
	cout << "CaseX : " << robot->getCaseX() << "\n";
	cout << "CaseY : " << robot->getCaseY() << "\n";
	cout << "PositionX : " << robot->getPositionX() << "\n";
	cout << "PositionY : " << robot->getPositionY() << "\n";


}



