
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

/*DEFINITION DES METHODES*/
void Robot::avancerRobot(double distance){
    
    try{

	/*CALCUL DE LA NOUVELLE POSITION*/
	double deltaX = - math.abs(distance) * math.sin(o);
	X=X+deltaX;

	double deltaY = + math.abs(distance) * math.cos(o);
	Y=Y+deltaY;

	/*ACTUALISATION DE LA CASE*/
	caseX=floor(X/(grille->getResolution()));
	caseY=floor(Y/(grille->getResolution()));

	/*VERIFICATION DE LA NOUVELLE POSITION*/
	double tailleGrille = (grille->getTaille())*(grille->getResolution());
	if(X<0 || X>tailleGrille){throw coordonneesHorsGrille();}
	else if(Y<0 || Y>tailleGrille){throw coordonneesHorsGrille();}

	if(X<bordureSortie || X>tailleGrille-bordureSortie){throw coordonneesHorsBordure();}
	else if(Y<bordureSortie || Y>tailleGrille-bordureSortie){throw coordonneesHorsBordure();}

}
catch(coordonneesHorsGrille& e){
	cout << "Il y a un probleme.\n";
}
catch(coordonneesHorsBordure& e){
	cout << "Il va falloir translater la grille\n";
}
    
}

void Robot::avancerRobot(int dist) {
    double angle = robot.o * 2 * math.pi / 360;
    x += math.cos(angle)*dist;
    y += math.sin(angle)*dist;

    // CHECK SORTIE DE GRILLE => translation (ne pas oublier de translater le robot avec)
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
        cout << robot.x << endl;
        delay(1000);
    }

    return 0;
}



bool Robot::calculer_chemin(double direction)
{

}


