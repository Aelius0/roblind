#ifndef CCAPTEURIR_H
#define CCAPTEURIR_H
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <softPwm.h>
#include <iostream>



class CapteurIR
{


public :
    CapteurIR();
    int detection();



};


#endif // CCAPTEURIR_H
