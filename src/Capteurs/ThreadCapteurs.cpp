
/*
* File type : package body
* Package   : Capteurs

*/
//package header file
#include"../Principal/Robot.h"

void actualiserLaser(double inclinaison, double distance){

    /*CALCUL DE LA DISTANCE THEORIE ATTENDUE*/
    double distanceTheorique = hauteurLaser_ / cos(inclinaison);

    /*COMPARAISON AVEC LA DISTANCE MESUREE*/
    double deltaTheorique = distance - distanceTheorique;

    /*DETERMINATION DE L'EMPLACEMENT SUR LA GRILLE*/
    double projectionLaterale = distance * sin(inclinaison);

        /*DETERMINISATION DE LA POSITION*/
        double positionObstacleXrel = - projectionLaterale * sin(orientation_);
        double positionObstacleYrel = - projectionLaterale * cos(orientation_);
        double positionObstacleXabs = positionX_+positionObstacleXrel;
        double positionObstacleYabs = positionY_+positionObstacleYrel;

        /*DETERMINATION DE LA CASE CORRESPONDANTE*/
        int caseObstacleX = floor(positionObstacleXabs/(grille_->getResolutionX()));
        int caseObstacleY = floor(positionObstacleYabs/(grille_->getResolutionY()));

    /*ECRITURE DE L'OBSTACLE SUR LA GRILLE*/
    if(deltaTheorique>0){grille_->set(caseObstacleX,caseObstacleY,OBSTACLE);}
    else{grille_->set(caseObstacleX,caseObstacleY,CHAUSSEE);}

    }


void *ThreadCapteur (void *)
{
   cout<<"Thread capteur started !"<<endl;
}
