#include <QCoreApplication>
#include "roblind.h"
#include "AStar.h"
#include <pthread.h>
using namespace std;

int main(int argc, char *argv[])
{
    cout << "INITIALIZING..." << endl;

    QCoreApplication a(argc, argv);

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
//        strcpy(config.confBaseRoulante.numPort,"/dev/ttyAMA0"); // CHANGE POUR NOTRE RBPI
        strcpy(config.confBaseRoulante.numPort,"/dev/ttyS0");
        config.confBaseRoulante.numPinRelai=26;
        config.mode = manuel; // TEMP

    CRoblind robot(config);

    cout << "INITIALIZATION COMPLETED" << endl;

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
    pthread_t threadGPS;
    pthread_create(&threadGPS, NULL, &cgps::relaiBoucleGPS, robot.m_gps);
//    pthread_t pthreadKinect ();
//    pthread_t pthreadLidar ();

    while(true)
     {
                /*
                     if(!robot.detecterObst() && robot.captPresence())
                     {
                         robot.modeManuel();
                     }
                */
                    if(!robot.captPresence())
                    {
                        robot.m_baseRoulante->stop();
//                        robot.m_canne->m_buzzer->sonner(); // TEMP DISABLED
                    } else
                    {
                        robot.m_canne->m_buzzer->silence();

                    }

                    if(robot.detecterObst())
                    {
                        robot.m_canne->m_vibreur->vibrer();
                    } else
                    {
                        robot.m_canne->m_vibreur->arreter();
                    }
        delay(50);
    }


     /*  AStar esquive(&robot);

           while(1)
            {
               switch(mode)
               {
                    case manuel:
                            if(!robot.detecterObst() && robot.captPresence())
                            {
                                robot.m_canne->m_buzzer->silence();
                                robot.m_canne->m_vibreur->arreter();
                                robot.modeManuel();
                            }

                           if(!robot.captPresence())
                           {
                               robot.m_baseRoulante->stop();
                               robot.m_canne->m_buzzer->sonner();
                           }

                           if(robot.detecterObst())
                           {
                               robot.m_baseRoulante->stop();
                               robot.m_canne->m_vibreur->vibrer();
                           }
                           break;


                    case semiAuto :

                           if(!robot.detecterObst() && robot.captPresence())
                           {
                               robot.m_canne->m_buzzer->silence();
                               robot.m_canne->m_vibreur->arreter();
                               robot.modeManuel();
                           }

                          if(!robot.captPresence())
                          {
                              robot.m_baseRoulante->stop();
                              robot.m_canne->m_buzzer->sonner();
                          }

                          if(robot.detecterObst())
                          {
                              robot.m_baseRoulante->stop();
                              robot.m_canne->m_vibreur->vibrer();
                          }

                           break;

                    case automatique :
                    
                   
                             robot.m_gps->lirefichierXML();
                             int i =0;
                                   
                           while(robot.m_gps->latitudeXML[i]!=0&&robot.m_gps->longitudeXML[i]!=0)
                           {
                               
                               if(!robot.detecterObst() && robot.captPresence())
                               {
                                   robot.m_canne->m_buzzer->silence();
                                   robot.m_canne->m_vibreur->arreter();
                                   robot.modeAuto(i);
                                   i++;
                               }
                               
    
                              if(!robot.captPresence())
                              {
                                  robot.m_baseRoulante->stop();
                                  robot.m_canne->m_buzzer->sonner();
                              }
    
                              if(robot.detecterObst())
                              {
                                  robot.m_baseRoulante->stop();
                                  robot.m_canne->m_vibreur->vibrer();
                              }
 
                                       
                                       
                           }
                           
                   break;
 
                }
            }

           //setup map
          esquive.generateRandomGrid(time(NULL), 120);




           int xA, yA, xB, yB;
           xA = n / 2;
           yA = 0;
           xB = n/2 - 20 + rand() % 40;
           yB = m - 1;


           clock_t start = clock();
           string route = esquive.AStarAlgo(xA, yA, xB, yB);
           clock_t end = clock();
           float time_elapsed = (float)(end - start)* 1000.0 / CLOCKS_PER_SEC;
           //~ printf("Time to calculate : %f\n", time_elapsed);
           printf("%f\n", time_elapsed);
           cout << route << endl;

           // follow the route on the map and display it
           int length = route.length();
           if(length>0)
           {
               int j; char c;
               int x=xA;
               int y=yA;
               esquive.map[x][y]=2;
               for(int i=0;i<length;i++)
               {
                   c =route[i];
                   j=atoi(&c);
                   x=x+esquive.dx[j];
                   y=y+esquive.dy[j];
                   esquive.map[x][y]=3;
               }
               esquive.map[x][y]=4;


           }
           esquive.displayMap(); */




    exit (0);
    return a.exec();
}
