
//package header file
#include "ThreadMoteur.h"

void *ThreadMoteur (void *)
{
   using namespace std;
   cout<<"Thread moteur started !"<<endl;
   QCoreApplication a(argc, argv);

   baseRoulante("/dev/ttyAMA0", NUM_PIN_BASE_ROULANTE);

   int* dist = 0;

   while(true)
   {
       if (active) {
           if (noWay) {
               if (robot.calculerChemin()) {
                   noWay = false;
                   canne.m_vibreur->arreter();
               }
           }
           else {
               baseRoulante.getDistanceParcourue(dist, cm);
               if (*dist >= lastCmd) {
                   grille.avancerRobot(*dist - _pos);
                   if (robot.obstacleDevant()) {
                       if (!robot.calculer_chemin()) {
                           noWay = true;
                           canne.m_vibreur->vibrer();
                       }
                   } else {
                       if (path.size() == 0)
                           active = false;          // TODO : vibrer
                       else {
                           vector<int> cmd = path.pop();
                           lastCmdType = cmd[0];
                           lastCmd = cmd[1];
                           switch (cmd[0]) {
                           case -1: // TRANSLATION : reculer
                               ; // UNUSED
                           case 0: // TRANSLATION : avancer
                               m = drive;
                               baseRoulante.allerEn(cmd[1], avancer, speed, cm);
                               break;
                           case 1: // ROTATION
                               m = turn;
                               baseRoulante.tourner(cmd[1], rotationSpeed);
                               break;
                           }
                       }
                   }
               }
               else {
                   if (!robot.calculer_chemin()) {
                       // AUCUN CHEMIN
                       noWay = true;
                       canne.m_vibreur->vibrer();
                   }
               }
           }
       }
       else {
           ; // si inactif : détruire file ?
       }
       delay(5);
   }

   exit (0);
}
