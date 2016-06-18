#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <wiringPi.h>
#include <wiringSerial.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <QString>
#include <unistd.h>

using namespace std;

class CSerialPort
{

private :
    int m_fd;

public:
    CSerialPort();
    int open(const char * device, int baud);
    void close();
    bool putchar (char c);
    bool puts (const char * command);
    int dataAvailable();
    bool getchar(char *car);
    bool gets(char *reponse, unsigned int taille);
    void flush ();
};

#endif // SERIALPORT_H
