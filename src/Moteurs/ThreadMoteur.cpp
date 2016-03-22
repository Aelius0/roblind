
//package header file
#include "ThreadMoteur.h"
#include "../Principal/Robot.h"
#include "kangaroo.h"

/*
int* position = new int;
int lastCmd = -1;
int lastCmdType = -1;
*/

void *ThreadMoteur (void *)
{
   using namespace std;
   cout<<"Thread moteur started !"<<endl;
   QCoreApplication a(argc, argv);
   Ckangaroo baseRoulante("/dev/ttyAMA0");
   while(true)
   {
    err = baseRoulante.getPosition(mode.turn, position);
    if (!err) {
        if (path.size() == 0)
            ;          // TODO : vibrer
        else {
            vector<int> cmd = path.pop();
            switch (cmd[0]) {
            case 0: // TRANSLATION
                baseRoulante.allerEn(cmd[1],speed,cm);
                break;
            case 1: // ROTATION
                baseRoulante.tourner(cmd[1],rotationSpeed);
                break;
            }
        }
    }
    delay(10);
   }

   exit (0);
}
