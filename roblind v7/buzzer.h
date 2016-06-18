#ifndef BUZZER_H
#define BUZZER_H
#include"wiringPi.h"
#include"vibreur.h"

class CBuzzer
{
private:
    int m_pin;


public:
    CBuzzer(int pin);
    void sonner();
    void silence();

};

#endif // BUZZER_H
