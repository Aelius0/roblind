#include "CcapteurIR.h"

CapteurIR::CapteurIR()
{
    //if(wiringPiSetup() == -1)
        //exit (1) ;
    pinMode(12,INPUT);
}
int CapteurIR::detection()
{
        return digitalRead(12);
}
