
/*
 * File Type: Class body
 * Class: Robot
 */


//class header file
#include "Principal/Robot.hxx"

namespace Principal {
	
void react()
{
	digitalWrite(9, digitalRead(2))
}
	
int main(int argc, char *argv[])
{
	
    wiringPiSetup();
    
    
    pinMode(9, OUTPUT);
    pinMode(2, INPUT);
    pullUpDnControl(2, PUD_UP);

    int test_event = wiringPiISR(2, INT_EDGE_FALLING, void *react())
    
    while(true)
    {
//        digitalWrite(9, !digitalRead(2));
        delay(20);
    }
    return 0;
}


void Robot::marquer_bloque(int x, int y)
{
	grille_espace[x][y] = 1;
}


void Robot::marquer_libre(int x, int y)
{
	grille_espace[x][y] = 0;
}


void Robot::calculer_chemin(double direction)
{
//begin of modifiable zone(Cxx.Code)......C/b023ce1d-fe98-4d92-9cb1-a798f3f0c794
// TODO complete code
//end of modifiable zone(Cxx.Code)........E/b023ce1d-fe98-4d92-9cb1-a798f3f0c794

} // end calculer_chemin



} // namespace Principal
