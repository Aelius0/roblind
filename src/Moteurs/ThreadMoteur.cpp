
/*
* File type : package body
* Package   : Moteurs

*/
//package header file
#include "ThreadMoteur.h"
#include "../Principal/Robot.h"
#include "Moteur.h"

void *ThreadMoteur (void *)
{
   cout<<"Thread moteur started !"<<endl;
    robot.x = 0;
   while(true)
   {
    robot.x ++;
    delay(500);
   }
}
