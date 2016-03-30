
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
    canne = new CCanne();

	/*INITIALISATION DU PLACEMENT DU ROBOT*/
	// On place le robot au milieu de la grille.
	X=taille*resolution/2;
	caseX=floor(x/resolution);

	Y=taille*resolution/2;
	caseY=floor(y/resolution);

	positionInitialeXY = taille*resolution/2;
	caseInitialeXY = floor(taille/2);

	o=0;
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
    delete canne;
	cout << "Fin de la destruction du robot.\n";
}

/*DEFINITION DES METHODES*/
void Robot::avancerRobot(double distance){

	try{

	/*CALCUL DE LA NOUVELLE POSITION*/
	double deltaX = - math.abs(distance) * math.sin(o);
	x+=deltaX;

	double deltaY = + math.abs(distance) * math.cos(o);
	y+=deltaY;

	/*ACTUALISATION DE LA CASE*/
	caseX=math.floor(x/(grille->getResolution()));
	caseY=math.floor(y/(grille->getResolution()));

	/*VERIFICATION DE LA NOUVELLE POSITION*/
	double tailleGrille = (grille->getTaille())*(grille->getResolution());
	if(x<0 || x>tailleGrille){throw coordonneesHorsGrille();}
	else if(y<0 || y>tailleGrille){throw coordonneesHorsGrille();}

	if(x<bordureSortie || x>tailleGrille-bordureSortie){throw coordonneesHorsBordure();}
	else if(y<bordureSortie || y>tailleGrille-bordureSortie){throw coordonneesHorsBordure();}

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
	double Xobstacle = x + distObstacle * math.cos(o) ;
	double Yobstacle = y + distObstacle * math.sin(o) ;

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
	double deltaX = x - positionInitialeXY;
	double deltaY = y - positionInitialeXY;

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
	x = Xentree;
	caseX = caseXentree;
	y = Yentree;
	caseY = caseYentree;

	cout << "-------------------------------------\n";

}

bool Robot::calculer_chemin(double direction = getDirection(), double dist = 150)
{
	stop();
	
	int xa = static_cast<int>(x + dist*math.cos(o));
	int ya = static_cast<int>(y + dist*math.sin(o));
	parse_path(static_cast<int>(x), static_cast<int>(y), xa, ya);
}

bool Robot::obstacleDevant(int dist = 20)
{
    int dx = static_cast<int>(math.cos(o)*dist);
    int dy = static_cast<int>(math.sin(o)*dist);
    if (o < math.pi) {
        if (o < math.pi/2 || o > (3/2)*math/pi)) {
            for (int i = 0; i < dx; ++i)
                for (int j = 0; j < dy; ++j)
                    if (grille_espace.get(x + i, y + j) > 0)
                        return true;
        }
        else {
            for (int i = dx; i < 0; ++i)
                for (int j = 0; j < dy; ++j)
                    if (grille_espace.get(x + i, y + j) > 0)
                        return true;
        }
    } else {
        for (int j = 0; j < dy; ++j)
            if (o < math.pi/2 || o > (3/2)*math.pi)) {
                for (int i = 0; i < dx; ++i)
                    if (grille_espace.get(x + i, y + j) > 0)
                        return true;
            } else {
                for (int i = dx; i < 0; ++i)
                    if (grille_espace.get(x + i, y + j) > 0)
                        return true;
            }
    }
}

double Robot::getDirection(){
	
	
}


bool Robot::obstacleDevant(int dist = 10)
{
    double angle = robot.o * 2 * math.pi / 360;
    int dx = static_cast<int>(math.cos(angle)*dist);
    int dy = static_cast<int>(math.sin(angle)*dist);
    if (o < 180) {
        if (o < 90 || o > 270) {
            for (int i = 0; i < dx; ++i)
                for (int j = 0; j < dy; ++j)
                    if (grille_espace.get(x + i, y + j) > 0)
                        return true;
        }
        else {
            for (int i = dx; i < 0; ++i)
                for (int j = 0; j < dy; ++j)
                    if (grille_espace.get(x + i, y + j) > 0)
                        return true;
        }
    } else {
        for (int j = 0; j < dy; ++j)
            if (o < 90 || o > 270) {
                for (int i = 0; i < dx; ++i)
                    if (grille_espace.get(x + i, y + j) > 0)
                        return true;
            } else {
                for (int i = dx; i < 0; ++i)
                    if (grille_espace.get(x + i, y + j) > 0)
                        return true;
            }
    }
}

bool Robot::stop() {
    baseRoulante.stop();
	delete path;
	path = new queue< vector<int> >();
}
