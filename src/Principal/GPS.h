
#ifndef __GPS_f0416f69_cd1e_454e_85b6_ebab2b213a03_H_INCLUDED
#define __GPS_f0416f69_cd1e_454e_85b6_ebab2b213a03_H_INCLUDED

/*
 * File Type: Class header
 * Class: GPS
 */

//owner package header file
//includes for used library types
#include <string>
#include <vector>

class GPS
{
 public:
void lire_destination();
double get_direction();

 private:
    std::string destination;
    std::vector<double> position;
    void* chemin;

}; //end class GPS
#endif // __GPS_f0416f69_cd1e_454e_85b6_ebab2b213a03_H_INCLUDED
