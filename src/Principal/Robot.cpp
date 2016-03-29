
//class header file
#include "Robot.h"
#include "../Moteurs/ThreadMoteur.h"
#include "../Capteurs/ThreadCapteurs.h"

using namespace std;
pthread_t motThread, capteurThread;
Robot robot;

/*DECLARATION DES STRUCTURES D'ERREURS POUR LES EXCEPTIONS*/
struct coordonneesHorsGrille{};
struct coordonneesHorsBordure{};

/*CONSTRUCTEUR ET DESTRUCTEUR*/
Robot::Robot(double resolution = RESOLUTION, int taille = DIMENSION){

	cout << "----------------------\n";
	cout << "CONSTRUCTION DU ROBOT.\n";

	/*CREATION DE LA GRILLE*/
	grille = new Grille(resolution,taille); // sert à repérer les obstacles

	/*INITIALISATION DU PLACEMENT DU ROBOT*/
	// On place le robot au milieu de la grille.
	X=taille*resolution/2;
	caseX=floor(X/resolution);

	Y=taille*resolution/2;
	caseY=floor(Y/resolution);

	positionInitialeXY = taille*resolution/2;
	caseInitialeXY = floor(taille/2);

	orientation=0;
	vitesse=0;

	/*DEFINITON DES BORDURES D'ENTREE ET DE SORTIE*/
	bordureEntree = taille*resolution/5; // A REDEFINIR
	bordureSortie = taille*resolution/10; // A REDEFINIR

	cout << "Fin de la construction du robot.\n";

}

Robot::~Robot(){
	cout << "---------------------------------\n";
	cout << "DESTRUCTION DU ROBOT\n";
	delete grille;
	cout << "Fin de la destruction du robot.\n";
}

/*DEFINITION DES METHODES*/
void Robot::avancerRobot(double distance){

	try{

	/*CALCUL DE LA NOUVELLE POSITION*/
	double deltaX = - math.abs(distance) * math.sin(orientation);
	X+=deltaX;

	double deltaY = + math.abs(distance) * math.cos(orientation);
	X+=deltaY;

	/*ACTUALISATION DE LA CASE*/
	caseX=math.floor(X/(grille->getResolution()));
	caseY=math.floor(Y/(grille->getResolution()));

	/*VERIFICATION DE LA NOUVELLE POSITION*/
	double tailleGrille = (grille->getTaille())*(grille->getResolution());
	if(X<0 || X>tailleGrille){throw coordonneesHorsGrille();}
	else if(Y<0 || Y>tailleGrille){throw coordonneesHorsGrille();}

	if(X<bordureSortie || X>tailleGrille-bordureSortie){throw coordonneesHorsBordure();}
	else if(Y<bordureSortie || Y>tailleGrille-bordureSortie){throw coordonneesHorsBordure();}

	}
	
	/*GESTION DES EXCEPTIONS*/
	catch(coordonneesHorsGrille& e){
		cout << "On est sorti de la grille. C'est pas bien. \n";
	}
	catch(coordonneesHorsBordure& e){
		cout << "Il va falloir translater la grille. APPELER LA BONNE METHODE.\n";
	}
    
}

void Robot::actualiserLaser(double distMesuree, double inclinaison){

	/*DISTANCE THEORIQUE ATTENDUE*/
	double distTheorique = hauteurLaser / math.cos(inclinaison) ;
	// L'inclinaison se mesure par rapport à la normale verticale
	// En regadant vers l'avant du robot, l'inclinaison est positive
	// quand on se penche vers la droite

	/*REPERAGE DE LA DISTANCE LATERALE DE L'OBSTACLE*/
	double distObstacle = distMesuree * math.sin(inclinaison) ;
	// Distance positive si l'obstacle est à droite
	// Distance négative si l'obstacle est à gauche

	/*REPERAGE DE LA POSITION ET DE LA CASE*/
	double Xobstacle = X + distObstacle * math.cos(orientation) ;
	double Yobstacle = Y + distObstacle * math.sin(orientation) ;

	int caseObstacleX = floor(Xobstacle/grille->getResolution());
	int caseObstacleY = floor(Yobstacle/grille->getResolution());

	/*MODIFICATION DE LA GRILLE*/
	double deltaDist = distMesuree - distTheorique ;
	double margeTolerance = 0.1*distTheorique; // Marge de tolerance de 10% de la distance theorique
	if(deltaDist>=margeTolerance){
		grille->set(caseObstacleX,caseObstacleY,CHAUSSEE);
	}
	else if(abs(deltaDist)<margeTolerance){
		grille->set(caseObstacleX,caseObstacleY,RAS);
	}
	else{
		grille->set(caseObstacleX,caseObstacleY,OBSTACLE);
	}
}

void Robot::translaterGrille(){

	cout << "-------------------------------------\n";
	cout << "TRANSLATION DE LA GRILLE\n";

	/*CALCUL DES COORDONNEES D'ENTREE DESIREES*/
	double deltaX = X - positionInitialeXY;
	double deltaY = Y - positionInitialeXY;

	double numerateurRapport = (grille->getTailleGrille()/2)-bordureEntree;
	double denominateurRapport = max(math.abs(deltaX),math.abs(deltaY));
	double rapportHomothetie = numerateurRapport/denominateurRapport;
	// La position de respawn est sur la bordure d'entree, inclue dans la bordure de sortie
	// Sur la carre definie par la position de sortie, on determine la position symetrique par rapport au centre
	// On se ramene sur la bordure d'entree avec le rapport d'homothetie entre les deux bordures
	// Cette homothetie a pour centre le centre de la grille

	double Xentree = positionInitialeXY - deltaX*rapportHomothetie;
	int caseXentree = math.floor(Xentree/grille->getResolution());
	double Yentree = positionInitialeXY - deltaY*rapportHomothetie;
	int caseYentree = math.floor(Yentree/grille->getResolution());

	/*DETERMINATION DES VECTEURS DE TRANSLATION ET TRANSLATION*/
	int translationCaseX = caseXentree - caseX;
	int translationCaseY = caseYentree - caseY;
	grille->translaterRepereGrille(translationCaseX,translationCaseY);

	/*ACTUALISATION DES POSITIONS DU ROBOT*/
	X = Xentree;
	caseX = caseXentree;
	Y = Yentree;
	caseY = caseYentree;

	cout << "-------------------------------------\n";

}

int main(int argc, char *argv[])
{
    cout << "Main robot code" << endl;
    wiringPiSetup();
    grille = new Grille(resolutionX,resolutionY,tailleX,tailleY);
    pthread_create(&motThread, NULL, ThreadMoteur, NULL);
    pthread_create(&capteurThread, NULL, ThreadCapteur, NULL);

    pinMode(9, OUTPUT);
    pinMode(2, INPUT);
    pullUpDnControl(2, PUD_UP);

    //int test_event = wiringPiISR(2, INT_EDGE_BOTH, &react);
    
    while(true)
    {
//        digitalWrite(9, !digitalRead(2));
        cout << robot.X << endl;
        delay(1000);
    }

    return 0;
}



bool Robot::calculer_chemin(double direction)
{
	
}

bool Robot::obstacleDevant(int dist = 10)
{
    int dx = static_cast<int>(math.cos(orientation)*dist);
    int dy = static_cast<int>(math.sin(orientation)*dist);
    if (orientation < math.pi) {
        if (orientation < math.pi/2 || orientation > (3/2)*math/pi)) {
            for (int i = 0; i < dx; ++i)
                for (int j = 0; j < dy; ++j)
                    if (grille_espace.get(X + i, Y + j) > 0)
                        return true;
        }
        else {
            for (int i = dx; i < 0; ++i)
                for (int j = 0; j < dy; ++j)
                    if (grille_espace.get(X + i, Y + j) > 0)
                        return true;
        }
    } else {
        for (int j = 0; j < dy; ++j)
            if (orientation < math.pi/2 || orientation > (3/2)*math.pi)) {
                for (int i = 0; i < dx; ++i)
                    if (grille_espace.get(X + i, Y + j) > 0)
                        return true;
            } else {
                for (int i = dx; i < 0; ++i)
                    if (grille_espace.get(X + i, Y + j) > 0)
                        return true;
            }
    }
}

double Robot::getDirection(){
	
	
}


