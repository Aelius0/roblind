
#include "roblind.h"


//~ #include <libfreenect/libfreenect.hpp>
#include "libfreenect/libfreenect.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <unistd.h>

#include <opencv2/opencv.hpp>
//parametres intrinseques de la Kinect
const double fx = 595.17079417;
const double fy = 595.17079417;
const double fxrgb = 525.70984318;
const double fyrgb = 525.70984318;
const double cu = 328.43206958;
const double cvb = 251.7538;
double baseline = 0.07381;
double shift_offset = 1090.7479;
double shift_scale = 0.125;
double Xprecedent = -10;
double Yprecedent = -10;
double THETAprecedent = 0;
double minDistance = 0.10;   //minimal x-travel/y-travel necessary to process frame
double minTheta = 10;       //minimal theta-variation (in degres) necessary to process frame
double rgbTimeDiff = 45000000;

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
        Kinect(float HauteurPersonne, float HauteurKinect, CRoblind *Robot, Grille *grille);
		cv::Mat modifyImage(Mat depthMat);
		void marquer_obstacles(Mat mask);
		int run_kinect();
	private:
        CRoblind *robot_;
		float hauteurPersonne_;
		float hauteurKinect_;
        Grille *grille_;
};

