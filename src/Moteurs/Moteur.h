
#ifndef __Moteur_08bd5c6a_318d_4483_b1cc_e6f4b5f6065b_H_INCLUDED
#define __Moteur_08bd5c6a_318d_4483_b1cc_e6f4b5f6065b_H_INCLUDED

/*
 * File Type: Class header
 * Class: Moteur
 */

//owner package header file
#include "Moteurs.h"


namespace Moteurs {




class Moteur
{
 public:
void avancer(double distance);
void tourner(double angle);
void arreter();

 private:
    double vitesse;

}; //end class Moteur

} // namespace Moteurs
#endif // __Moteur_08bd5c6a_318d_4483_b1cc_e6f4b5f6065b_H_INCLUDED
