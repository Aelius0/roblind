
#ifndef __Kinect_5c6b3e3f_fcd7_44fe_8410_f80405f349c8_H_INCLUDED
#define __Kinect_5c6b3e3f_fcd7_44fe_8410_f80405f349c8_H_INCLUDED

/*
 * File Type: Class header
 * Class: Kinect
 */

//owner package header file
#include "Capteurs/Capteurs.hxx"


namespace Capteurs {




class Kinect
{
 public:
void marquer_obstacles();

 private:
    void* profondeur;
    void* image;

}; //end class Kinect

} // namespace Capteurs
#endif // __Kinect_5c6b3e3f_fcd7_44fe_8410_f80405f349c8_H_INCLUDED