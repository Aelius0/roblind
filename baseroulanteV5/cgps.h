#ifndef CGPS_H
#define CGPS_H

#include <iostream>
#include <QFile>
#include <QtXml/QtXml>
#include <QString>
#include <QProcess>
#include <limits>

using namespace std;

class cgps
{


public:

    struct positionOrigine
    {
        double  longitude;
        double  latitude;

    };

    struct positionDestination
    {

        double  longitude;
        double  latitude;

    };
    positionOrigine posOrigine;
    positionDestination posDestination;

    double latitudeXML[100];
    double longitudeXML[100];



    //~CGPS();
    cgps();
    void getPositionActuel();
    double getDistance();
    double getAngle();
    void lirefichierXML();

    static void* relaiBoucleGPS(void* gps);
    void boucleGPS();
};

#endif // CGPS_H
