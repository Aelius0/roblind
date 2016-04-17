
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
Robot::Robot(int taille = DIMENSION, double resolution = RESOLUTION){

	cout << "----------------------\n";
	cout << "CONSTRUCTION DU ROBOT.\n";

	/*CREATION DE LA GRILLE*/
	grille = new Grille(taille, resolution); // sert à repérer les obstacles

    	m_canne = new CCanne();
  	m_baseRoulante= new Cbaseroulante("/dev/ttyAMA0",27);
    	m_lidarLite = new lidarLite();

	/*INITIALISATION DU PLACEMENT DU ROBOT*/

	x=grille->getDimension()/2;
	y=grille->getDimension()/2;

	positionInitialeXY = grille->getDimension()/2;
	//cout << "positionInitialeXY :" << positionInitialeXY << "\n";
	caseInitialeXY = floor(grille->getDimension()/2);
	//cout << "CaseInitialeXY : " << caseInitialeXY << "\n";
	
	o=0;
	v=0;
	en_evitement = true;

	/*DEFINITON DES BORDURES D'ENTREE ET DE SORTIE*/
	bordureEntree = grille->getDimension()/5; // A REDEFINIR
	bordureSortie = grille->getDimension()/10; // A REDEFINIR

	cout << "Fin de la construction du robot.\n";

}

Robot::~Robot(){
	cout << "---------------------------------\n";
	cout << "DESTRUCTION DU ROBOT\n";
	delete grille;
   	delete m_canne;
	delete m_baseRoulante;
	delete m_lidarLite;
	cout << "Fin de la destruction du robot.\n";
}



/*DEFINITION DES METHODES*/
bool Robot::allerEn()
{
   position p;
    p = m_canne->m_joystick->getPosition();

    if ( p == 0) // avancer
    {
         m_baseRoulante->allerEn(100,avancer,1,cm);
    }

    if (p == 1) //reculer
    {
        m_baseRoulante->allerEn(100,reculer,1,cm);
    }

    if (p == 2) //aller à gauche
    {
        m_baseRoulante->tourner(90);
        m_baseRoulante->allerEn(100,avancer,1,cm);
    }

    if (p == 3) //aller a droite
    {
        m_baseRoulante->tourner(-90);
        m_baseRoulante->allerEn(100,avancer,1,cm);
    }

    if (p == 4) //stop
    {
        m_baseRoulante->stop();
    }
}


void Robot::avancerRobot(double distance){

	try{

	/*CALCUL DE LA NOUVELLE POSITION*/
	double deltaX = - abs(distance) * sin(o) / grille->getResolution();
	x+=deltaX;

	double deltaY = + abs(distance) * cos(o) / grille->getResolution();
	y+=deltaY;

	/*VERIFICATION DE LA NOUVELLE POSITION*/
	if(x<0 || x>grille->getDimension()){throw coordonneesHorsGrille();}
	else if(y<0 || y>grille->getDimension()){throw coordonneesHorsGrille();}

	if(x<bordureSortie || x>grille->getDimension()-bordureSortie){throw coordonneesHorsBordure();}
	else if(y<bordureSortie || y>grille->getDimension()-bordureSortie){throw coordonneesHorsBordure();}

	}

	/*GESTION DES EXCEPTIONS*/
	catch(coordonneesHorsGrille& e){
		translaterGrille();
		cout << "Sortie de la grille. \n";
	}
	catch(coordonneesHorsBordure& e){
		translaterGrille();
		cout << "Sortie des bordures.\n";
		cout << "On a translate la grille.\n";
	}

}

void Robot::actualiserLaser(double distMesuree, double inclinaison){

	/*DISTANCE THEORIQUE ATTENDUE*/
	double distTheorique = hauteurLaser / cos(inclinaison) ;
	// L'inclinaison se mesure par rapport à la normale verticale
	// En regadant vers l'avant du robot, l'inclinaison est positive
	// quand on se penche vers la droite

	/*REPERAGE DE LA DISTANCE LATERALE DE L'OBSTACLE*/
	double distObstacle = distMesuree * sin(inclinaison) ;
	// Distance positive si l'obstacle est à droite
	// Distance négative si l'obstacle est à gauche

	/*REPERAGE DE LA POSITION*/
	double Xobstacle = x + (distObstacle * cos(o) / grille->getResolution() ) ;
	double Yobstacle = y + (distObstacle * sin(o) / grille->getResolution() ) ;

	/*MODIFICATION DE LA GRILLE*/
	double deltaDist = distMesuree - distTheorique ;
	double margeTolerance = 0.1*distTheorique; // Marge de tolerance de 10% de la distance theorique
	if(deltaDist>=margeTolerance){
		grille->set(floor(Xobstacle),floor(Yobstacle),CHAUSSEE);
	}
	else if(abs(deltaDist)<margeTolerance){
		grille->set(floor(Xobstacle),floor(Yobstacle),RAS);
	}
	else{
		grille->set(floor(Xobstacle),floor(Yobstacle),OBSTACLE);
	}
}

void Robot::translaterGrille(){

	cout << "TRANSLATION DE LA GRILLE\n";

	/*CALCUL DES COORDONNEES D'ENTREE DESIREES*/
	double deltaX = x - positionInitialeXY;
	double deltaY = y - positionInitialeXY;
	cout << "Le deltaX vaut : " << deltaX << "\n";
	cout << "Le deltaY vaut : " << deltaY << "\n";

	double numerateurRapport = (grille->getDimension()/2)-bordureEntree;
	cout << "Le numerateur du rapport : " << numerateurRapport << "\n";
	double denominateurRapport = max(abs(deltaX),abs(deltaY));
	cout << "Le denominateur du rapport : " << denominateurRapport << "\n";
	double rapportHomothetie = numerateurRapport/denominateurRapport;
	cout << "Le rapport d'homothetie : " << rapportHomothetie << "\n";
	// La position de respawn est sur la bordure d'entree, inclue dans la bordure de sortie
	// Sur la carre definie par la position de sortie, on determine la position symetrique par rapport au centre
	// On se ramene sur la bordure d'entree avec le rapport d'homothetie entre les deux bordures
	// Cette homothetie a pour centre le centre de la grille

	double Xentree = positionInitialeXY - deltaX*rapportHomothetie;
	cout << "Xentree : " << Xentree << "\n";
	int caseXentree = floor(Xentree);
	cout << "CaseXEntree : " << caseXentree << "\n";
	double Yentree = positionInitialeXY - deltaY*rapportHomothetie;
	cout << "Yentree : " << Yentree << "\n";
	int caseYentree = floor(Yentree);
	cout << "CaseYEntree : " << caseYentree << "\n";

	/*DETERMINATION DES VECTEURS DE TRANSLATION ET TRANSLATION*/
	int translationCaseX = caseXentree - floor(x);
	int translationCaseY = caseYentree - floor(y);
	cout << "Vecteur de translation sur x : " << translationCaseX << "\n";
	cout << "Vecteur de translation sur y : " << translationCaseY << "\n";
	grille->translaterRepereGrille(translationCaseX,translationCaseY);

	/*ACTUALISATION DES POSITIONS DU ROBOT*/
	x = Xentree;
	y = Yentree;
	cout << "La nouvelle position du robot : [" << x << "," << y << "]\n";

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

bool Robot::obstacleDevant(int dist)
{
    int dx = static_cast<int>(cos(o)*dist);
    int dy = static_cast<int>(sin(o)*dist);
    if (o < 3.14/*math.pi*/) {
        if (o < 3.14/*math.pi*//2 || o > (3/2)*3.14/*math.pi*/) {
            for (int i = 0; i < dx; ++i)
                for (int j = 0; j < dy; ++j)
                    if (grille->get(x + i, y + j) > 0)
                        return true;
        }
        else {
            for (int i = dx; i < 0; ++i)
                for (int j = 0; j < dy; ++j)
                    if (grille->get(x + i, y + j) > 0)
                        return true;
        }
    } else {
        for (int j = 0; j < dy; ++j)
            if (o < 3.14/*math.pi*//2 || o > (3/2)*3.14/*math.pi*/) {
                for (int i = 0; i < dx; ++i)
                    if (grille->get(x + i, y + j) > 0)
                        return true;
            } else {
                for (int i = dx; i < 0; ++i)
                    if (grille->get(x + i, y + j) > 0)
                        return true;
            }
    }

    return true;
}

/*
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
*/
bool Robot::stop() {
    baseRoulante.stop();
	delete path;
	path = new queue< vector<int> >();
}
