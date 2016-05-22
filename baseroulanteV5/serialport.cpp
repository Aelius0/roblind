#include "serialport.h"
using namespace std;


CSerialPort::CSerialPort()
{

}

int CSerialPort::open(const char *device, int baud)
{
    m_fd=serialOpen(device,baud);
    //wiringPiSetup();
    return m_fd;
}


void CSerialPort::close()
{
    if (m_fd != -1 )
    {
        serialClose(m_fd);


    }
}

bool CSerialPort::putchar (char c)
{
    bool retour=false;
    if (m_fd != -1 )
    {
        if(write(m_fd,&c,1)==1)
            retour=true;
        else
            retour=false;
    }
    return retour;
}

bool CSerialPort::puts(const char *command)
{
    bool retour=false;
    if (m_fd != -1 )
    {
        if(write(m_fd,command,strlen(command))==strlen(command))
            retour=true;
        else
            retour=false;
    }
    return retour;
}

int CSerialPort::dataAvailable()
{
    if (m_fd != -1)
    {
        return serialDataAvail(m_fd) ;
    }
}
bool CSerialPort::getchar(char * car)
{
    bool retour=false;
    if (m_fd != -1 )
    {
        if(read(m_fd,car,1)==1)
            retour=true;

    }
    return retour;
}

bool CSerialPort::gets(char *reponse, unsigned int taille)
{
    bool retour=false;
    int nbCarRecu=0;
    if (m_fd != -1 )
    {
        nbCarRecu=read(m_fd,reponse,taille);
        if(nbCarRecu>0)
        {
            reponse[nbCarRecu]='\0';
            retour=true;
        }
        else
            retour=false;
    }
    return retour;
}

void CSerialPort::flush()
{
    if (m_fd != -1 )
    {
        serialFlush(m_fd);
    }
}
