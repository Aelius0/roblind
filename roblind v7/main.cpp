#include <QCoreApplication>
#include "roblind.h"
#include "AStar.h"
#include <pthread.h>
using namespace std;
//Roblind V7 : code final du roblind P14


int main(int argc, char *argv[])
{
    cout << "INITIALIZING..." << endl;
	//configuration du roblind
    configRoblind config;
	config.numPinIR=27;
	config.confAX13.pinServo1=23;
	config.confAX13.pinServo2=24;
	config.confCanne.confJoystick.x=AIN0;
	config.confCanne.confJoystick.y=AIN1;
	config.confCanne.confJoystick.numPinBouton=25;
	config.confCanne.numPinVibreur=22;
	config.confCanne.numPinBuzzer=21;
	config.confCanne.numCapteurForce=AIN3;
	config.confCanne.numCapteurPresence=AIN2;
	strcpy(config.confBaseRoulante.numPort,"/dev/ttyS0");
	config.confBaseRoulante.numPinRelai=26;
	config.mode = manuel; // pour le forum
    CRoblind robot(config);
    cout << "INITIALIZATION COMPLETE" << endl;
	
	
	//creation des différentes threads qui vont s'exécuter sur le roblind : mouvement, GPS, Kinect, displayKinect pour l'affichage (cf forum) et Lidar
    /* Syntaxe pour les threads :
     * type : pthread_t
     * create : pthread_create(...)
     *   Arg 1 : pointeur vers le thread
     *   Arg 2 : NULL (attributs spécifiques pour le thread : scheduling, etc...)
     *   Arg 3 : pointeur vers la fonction à appeler, de type void*
     *   Arg 4 : Arguments éventuels à passer à la fonction appelée
     */
    pthread_t threadMouvement;
    pthread_create(&threadMouvement, NULL, &CRoblind::executerMouvement, &robot);
    pthread_t threadGPS; //désactivée pour le forum
//  pthread_create(&threadGPS, NULL, &cgps::relaiBoucleGPS, robot.m_gps);
    pthread_t threadKinect;
    pthread_create(&threadKinect, NULL, &Kinect::runKinect, robot.m_Kinect);
    pthread_t displayThread;
    pthread_create(&displayThread, NULL, &Kinect::displayRun, NULL);
    pthread_t threadLidar;//désactivée pour le forum
//  pthread_create(&threadLidar, NULL, &lidarLite::runLidar, robot.m_rightLidar);
    pthread_t threadLidarMove;
    pthread_create(&threadLidarMove, NULL, &AX13::moveLidar, robot.m_pinServo);

    while(true)
     {

		if(!robot.captPresence())
		{
			robot.m_baseRoulante->stop();
			robot.m_canne->m_buzzer->sonner();
		} else
		{
			robot.m_canne->m_buzzer->silence();
			robot.boucleMouvement();
		}

		if(robot.detecterObst())
		{
			robot.m_canne->m_vibreur->vibrer();
		} else
		{
			robot.m_canne->m_vibreur->arreter();
		}
        delay(10);
    }

    exit (0);
    return 0;
}
