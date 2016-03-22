
//package header file
#include "ThreadMoteur.h"

void *ThreadMoteur (void *)
{
   using namespace std;
   cout<<"Thread moteur started !"<<endl;
   QCoreApplication a(argc, argv);
   Ckangaroo baseRoulante("/dev/ttyAMA0");
   while(true)
   {
       if (active) {
           err = baseRoulante.getPosition(m, position);
           if (!err && *position >= lastCmd) {
               if (path.size() == 0)
                   active = false;          // TODO : vibrer
               else {
                   vector<int> cmd = path.pop();
                   lastCmdType = cmd[0];
                   lastCmd = cmd[1];
                   switch (cmd[0]) {
                   case 0: // TRANSLATION
                       m = drive;
                       baseRoulante.allerEn(cmd[1],speed,cm);
                       break;
                   case 1: // ROTATION
                       m = turn;
                       baseRoulante.tourner(cmd[1],rotationSpeed);
                       break;
                   }
               }
           }
           else {
               ; // RESET PROTOCOL ??
           }
       }
       delay(10);
   }

   exit (0);
}
