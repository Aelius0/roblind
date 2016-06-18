#ifndef KINECT_H
#define KINECT_H


#include <libfreenect/libfreenect.hpp>
#include "libfreenect.hpp"
#include "Grille.h"
#include "baseroulante.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <unistd.h>

#include <opencv2/opencv.hpp>

//parametres intrinseques de la Kinect

using namespace cv;
using namespace std;

class myMutex {
	public:
		myMutex();
		void lock();
		void unlock();
	private:
		pthread_mutex_t m_mutex;
};

class MyFreenectDevice : public Freenect::FreenectDevice {
	public:
		MyFreenectDevice(freenect_context *_ctx, int _index);
		// Do not call directly even in child
		void VideoCallback(void* _rgb, uint32_t timestamp);
		// Do not call directly even in child
		void DepthCallback(void* _depth, uint32_t timestamp);
		bool getVideo(Mat& output);
		bool getDepth(Mat& output);
	private:
		std::vector<uint8_t> m_buffer_depth;
		std::vector<uint8_t> m_buffer_rgb;
		std::vector<uint16_t> m_gamma;
		myMutex m_rgb_mutex;
		myMutex m_depth_mutex;
		bool m_new_rgb_frame;
		bool m_new_depth_frame;
        Mat depthMat;
		Mat depthf;
		Mat rgbMat;
        Mat ownMat;
};

class Kinect {
    public:
         //static Kinect* mainKinect;

        Kinect(Grille* grille, positionBaseRoulante *pos);
        //cv::Mat modifyImage(Mat depthMat);
		void marquer_obstacles(Mat mask);
        static void callBackHandler(int sigNum);
        static void* runKinect(void* kinect);
        static void* displayRun(void*);
        void killKinect();

    private:
        double fx;
        double fy;
        double fxrgb;
        double fyrgb;
        double cu;
        double cvb;
        double baseline;
        double shift_offset;
        double shift_scale;
        double Xprecedent;
        double Yprecedent;
        double THETAprecedent;
        double minDistance;   //minimal x-travel/y-travel necessary to process frame
        double minTheta;       //minimal theta-variation (in degres) necessary to process frame
        double rgbTimeDiff;

        bool die_;
		float hauteurPersonne_;
		float hauteurKinect_;
        Grille* grille_;
        positionBaseRoulante* pos_;
};


#endif // KINECT_H
