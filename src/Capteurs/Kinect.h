
#ifndef __Kinect_5c6b3e3f_fcd7_44fe_8410_f80405f349c8_H_INCLUDED
#define __Kinect_5c6b3e3f_fcd7_44fe_8410_f80405f349c8_H_INCLUDED

/*
 * File Type: Class header
 * Class: Kinect
 */

//owner package header file

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "../Principal/Robot.h"
//#include "../Principal/Grille.h"

class Kinect
{
 public:
Kinect();
~Kinect();
//void Kinect::marquer_obstacles(int largeur, int hauteur);
int get_image(int argc, char **argv); //return 0 if ok else return 1

//fonctions pour faire tourner la Kinect
void modifyImage(int largeur, int longueur);
void* freenect_threadfunc(void *arg);
void rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp);
void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp);
void update_image();

 private:
uint8_t* profondeur;
uint8_t* couleur;
cv::Mat mask;

//trouver largeur et longueur des images: uint8** ?
    
}; //end class Kinect

#endif // __Kinect_5c6b3e3f_fcd7_44fe_8410_f80405f349c8_H_INCLUDED
