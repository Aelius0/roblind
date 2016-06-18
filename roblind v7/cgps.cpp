#include "cgps.h"

 const double PI=3.14159265359;
 const double R=6378.1370;

// Relai statique vers boucleGPS
void* cgps::relaiBoucleGPS(void* gps) {
    ((cgps*) gps)->boucleGPS();
    return 0;
}
void cgps::boucleGPS() {
    // Thread GPS
}

cgps::cgps()
{

    posOrigine.latitude=0;
    posOrigine.longitude=0;

    posDestination.latitude=0;
    posDestination.longitude=0;
}

double cgps::getAngle()
{

   /* double longDelta = qDegreesToRadians(posDestination.longitude - posOrigine.longitude);
    double y = qSin(longDelta) * qCos(qDegreesToRadians(posDestination.latitude));
    double x = qCos(qDegreesToRadians(posOrigine.latitude))*  qSin(qDegreesToRadians(posDestination.latitude)) - qSin(qDegreesToRadians(posOrigine.latitude))* qCos(qDegreesToRadians(posDestination.latitude))* qCos(longDelta);
    double angle = qRadiansToDegrees(qAtan2(y, x));
            while (angle < 0) {
                angle += 360;
            }
           return fmod(angle,360);

*/

}

double cgps::getDistance()
{
    double distance;
   // distance=R*qAcos(qSin(qDegreesToRadians(posOrigine.latitude))*qSin(qDegreesToRadians(posDestination.latitude))+qCos(qDegreesToRadians(posOrigine.latitude))*qCos(qDegreesToRadians(posDestination.latitude))*qCos(qDegreesToRadians(posOrigine.longitude)-qDegreesToRadians(posDestination.longitude)));
    return distance;


}

void cgps::getPositionActuel()
{
    QProcess OProcess;
    QString Command;    //Contains the command to be executed
    QStringList args;   //Contains arguments of the command

    Command = "./script";
    args<<"";

    OProcess.start(Command,args,QIODevice::ReadOnly); //Starts execution of command
    OProcess.waitForFinished();                       //Waits for execution to c

    QString latitude, longitude ;
    QFile lat("/tmp/gps.lat");
    QFile lon("/tmp/gps.lon");

    if(lat.open(QIODevice::ReadOnly|QIODevice::Text))
    {

        latitude = lat.readAll();
        posOrigine.latitude=latitude.toDouble();
        //cout<<endl<<latitude.toStdString()<<endl;

    }


    if(lon.open(QIODevice::ReadOnly|QIODevice::Text))
    {

        longitude = lon.readAll();
        posOrigine.longitude=longitude.toDouble();
        //cout<<result<<endl;

    }


}

void cgps::lirefichierXML()
{
QString filename="test.xml";

QFile fileXml(filename);

if(!fileXml.open(QFile::ReadOnly|QFile::Text))
{

    cout<<"fichier pas la "<<endl;
}
QXmlStreamReader reader;
reader.setDevice(&fileXml);
int i=0;
reader.readNext();
while(!reader.atEnd())
{
                if(reader.name()=="start_location")
                {
                        reader.readNextStartElement();
                        if(reader.name()=="lat")
                        {
                            latitudeXML[i]=reader.readElementText().toDouble();
                            cout<<latitudeXML[i]<<endl;
                        }
                        reader.readNextStartElement();
                        if(reader.name()=="lng")
                        {
                            longitudeXML[i]=reader.readElementText().toDouble();
                            cout<<longitudeXML[i]<<endl;
                            i++;
                        }
                }
                 reader.readNext();
}
fileXml.close();
}
