#include "kangaroo.h"


Ckangaroo::Ckangaroo(const char * portName)
{
    if( m_serialPort.open(portName,9600)==-1)
        m_isOpened=false;
    else
        m_isOpened=true;

}

bool Ckangaroo::init()
{
    bool retour=false;

    if(m_isOpened)
    {

        start(drive);
        m_serialPort.puts("D, UNITS 26704 mm = 711 lines\r\n");
        start(turn);
        m_serialPort.puts("T, UNITS 36000 degrees = 2384 lines\r\n");
        m_serialPort.puts("D,p0s0\r\n");
        m_serialPort.puts("T,p0s0\r\n");
    }
    return retour;
}


bool Ckangaroo::start(mode m)
{
    char commande[100]={0};
    bool retour=false;
    char cm;
    cm=(char)m;
    if(m_isOpened)
    {
        strcpy(commande,&cm);
        strcat(commande,",start\r\n");
        retour=m_serialPort.puts((const char*)commande);
    }
    return retour;
}

bool Ckangaroo::allerEn(int distance, int speed, unite u)
{
    char commande[100]={0};
    int position=0,  delta=0;
    QString tempo;
    bool retour  = false;
    init();
    start(drive);
    if(m_isOpened)
    {
        strcpy(commande,"D,p");
        distance=u*distance;
        tempo=QString::number(distance);
        strcat(commande,tempo.toStdString().c_str());
        strcat(commande,"s");
        speed=speed*384615;
        tempo=QString::number(speed);
        strcat(commande,tempo.toStdString().c_str());
        strcat(commande,"\r\n");
        retour=m_serialPort.puts(commande);
        do
        {
            getPosition(drive,position);
            cout<<"Position "<<position<<endl;
            delta=abs(distance-position);
        }while(delta>5);
    }

    return  retour;

}

bool Ckangaroo::tourner(double angle, double speed)
{

    char commande[100]={0};
     QString tempo;
    bool retour  = true;
    init();
    start(turn);
    if(m_isOpened)
    {
        strcpy(commande,"T,p");
        angle=angle*3558;
        tempo=QString::number(angle);
        strcat(commande,tempo.toStdString().c_str());
        strcat(commande,"s");
        speed=speed*384615;
        tempo=QString::number(speed);
        strcat(commande,tempo.toStdString().c_str());
        strcat(commande,"\r\n");
        retour=m_serialPort.puts(commande);
    }
    return  retour;


}

bool Ckangaroo::powerdown(const char * m)
{
    char commande[100]={0};
    bool retour=false;

    if(m_isOpened)
    {
        strcpy(commande,m);
        strcat(commande,",powerdown\r\n");
        retour=m_serialPort.puts((const char*)commande);
    }
    return retour;

}

//retourne un code erreur
//0 pas d'erreur
int Ckangaroo::getPosition(mode m,int &position)
{
    char commande[100]={0};
    char reponse[100]={0};
    int codeErreur=0;
    char tempo[10]={0};
    int nb=0;
    char cm=(char)m;

    if(m_isOpened)
    {

        strcpy(commande,&cm);
        strcat(commande,",getp\r\n");
        if(m_serialPort.puts(commande))
        {
            int i=0;
            do
            {
                if(nb=m_serialPort.dataAvailable()!=-1)
                {
                    m_serialPort.getchar(&reponse[i]);
                    i++;
                }
            }while(reponse[i-1]!='\n' && i<99);
            reponse[i]='\0';
            if(reponse[2]!='E')
            {
                int j=0;
                for( i=3;i<strlen(reponse)-2;i++)
                {
                    tempo[j++]=reponse[i];
                }
                position=atoi(tempo);
            }
            else
            {
                tempo[0]=reponse[2];
                tempo[1]=reponse[3];
                codeErreur=atoi(tempo);
            }
        }

    }
    return codeErreur;
}




