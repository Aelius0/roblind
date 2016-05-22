#include "Kinect.h"

using namespace cv;
using namespace std;

myMutex::myMutex() {
	pthread_mutex_init( &m_mutex, NULL );
}

void myMutex::lock() {
	pthread_mutex_lock( &m_mutex );
}

void myMutex::unlock() {
	pthread_mutex_unlock( &m_mutex );
}

MyFreenectDevice::MyFreenectDevice(freenect_context *_ctx, int _index):Freenect::FreenectDevice(_ctx, _index),m_buffer_depth(FREENECT_DEPTH_11BIT),m_buffer_rgb(FREENECT_VIDEO_RGB),m_gamma(2048),m_new_rgb_frame(false),m_new_depth_frame(false),depthMat(Size(640,480),CV_16UC1),rgbMat(Size(640,480),CV_8UC3, Scalar(0)),ownMat(Size(640,480),CV_8UC3,Scalar(0)) {	
	for( unsigned int i = 0 ; i < 2048 ; i++) {
		float v = i/2048.0;
		v = std::pow(v, 3)* 6;
		m_gamma[i] = v*6*256;
	}
}

// Do not call directly even in child
void MyFreenectDevice::VideoCallback(void* _rgb, uint32_t timestamp) {
	//~ std::cout << "RGB callback" << std::endl;
	m_rgb_mutex.lock();
	uint8_t* rgb = static_cast<uint8_t*>(_rgb);
	rgbMat.data = rgb;
	m_new_rgb_frame = true;
	m_rgb_mutex.unlock();
}

// Do not call directly even in child
void MyFreenectDevice::DepthCallback(void* _depth, uint32_t timestamp) {
	//~ std::cout << "Depth callback" << std::endl;
	m_depth_mutex.lock();
	uint16_t* depth = static_cast<uint16_t*>(_depth);
	depthMat.data = (uchar*) depth;
	m_new_depth_frame = true;
	m_depth_mutex.unlock();
}

bool MyFreenectDevice::getVideo(Mat& output) {
	m_rgb_mutex.lock();
	if(m_new_rgb_frame) {
		cv::cvtColor(rgbMat, output, CV_RGB2BGR);
		m_new_rgb_frame = false;
		m_rgb_mutex.unlock();
		return true;
	} else {
		m_rgb_mutex.unlock();
		return false;
	}
}

bool MyFreenectDevice::getDepth(Mat& output) {
	m_depth_mutex.lock();
	if(m_new_depth_frame) {
		depthMat.copyTo(output);
		m_new_depth_frame = false;
		m_depth_mutex.unlock();
		return true;
	} else {
		m_depth_mutex.unlock();
		return false;
	}
}

Kinect::Kinect(float HauteurPersonne, float HauteurKinect, CRoblind *Robot, Grille *grille){
	robot_ = Robot;
	hauteurPersonne_=HauteurPersonne;
	hauteurKinect_=HauteurKinect;
    grille_ = grille;
}

Mat Kinect::modifyImage(Mat depthMat) //supprimer les détections au delà de 1m
{
    Mat mask;
    Mat temp;
	threshold(depthMat,temp,1000,155,THRESH_TOZERO_INV); 
	erode(temp, mask, Mat());
    return mask;
}

void Kinect::marquer_obstacles(Mat mask)
{
	int X = 640;
	int Y = 480;
	
	for(int b=Y-1; b>=0; b--) // b = COLONNE SUR L'IMAGE MODIFIEE, EN PIXEL
	{
		for(int a = 0; a<X; a++) // a = LIGNE SUR L'IMAGE MODIFIEE, EN PIXEL
		{
			double profondeur = ((fx*baseline)/(shift_scale*(shift_offset-mask.at<ushort>(a,b))))* 1000;
			if(profondeur > 0 && profondeur < 3000)    //filtrage des points inutiles
			{
				//calcul des coordonnees par rapport à la Kinect en mm
				float xKinectbrut = (float) (a-cu)/fx * profondeur;
				float yKinectbrut = (float) (b-cvb)/fy * profondeur;
				float zKinectbrut =  profondeur;

				//~ std::cout << "a,b: " << a << " " << b << " x,y,z: " << xKinectbrut << " " << yKinectbrut << " " << zKinectbrut << " " << std::endl;   
				
				// CHANGEMENT DE REPERE D'ANGLE -ROBOT.O
                float theta = robot_->m_baseRoulante->m_posBR.alpha;
                float grilleX = xKinectbrut * cos(theta) - zKinectbrut * sin(theta);
                float grilleY = xKinectbrut * sin(theta) + zKinectbrut * cos(theta);
				float grilleHauteur = -yKinectbrut + hauteurKinect_;
				if (grilleHauteur<hauteurPersonne_ && grilleHauteur>0){
                    int obstacleX = static_cast<int>(robot_->m_baseRoulante->m_posBR.x + grilleX);
                    int obstacleY = static_cast<int>(robot_->m_baseRoulante->m_posBR.y + grilleY);
                    grille_->marquerObstacles(obstacleX,obstacleY);
					//~ robot.(*grille).grille_[static_cast<int>(robot.x + grilleX)][static_cast<int>(robot.y + grilleY)]=1; // OBSTACLE=1
				}
			}
		}
	}
}

int Kinect::run_kinect() {
	bool die(false);
	string filename("snapshot");
	string suffix(".png");
	//~ int i_snap(0),iter(0);
	
	Mat depthMat(Size(640,480),CV_16UC1);
	//~ Mat depthf(Size(640,480),CV_8UC1);
	//~ Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
	//~ Mat ownMat(Size(640,480),CV_8UC3,Scalar(0));
	Mat mask;
	
	// The next two lines must be changed as Freenect::Freenect
	// isn't a template but the method createDevice:
	// Freenect::Freenect<MyFreenectDevice> freenect;
	// MyFreenectDevice& device = freenect.createDevice(0);
	// by these two lines:
		
	Freenect::Freenect freenect;
	MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);
	
	//~ namedWindow("rgb",CV_WINDOW_AUTOSIZE);
	//~ namedWindow("depth",CV_WINDOW_AUTOSIZE);
	//~ device.startVideo();
	device.startDepth();
	while (!die) {
		//~ device.getVideo(rgbMat);
		if(!device.getDepth(depthMat))
		{
			usleep(20000);
			continue;
		}
		
		//~ cv::imshow("rgb", rgbMat);
		
		//~ depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
		//~ mask=modifyImage(depthMat);
		//~ cv::imshow("depth",depthf);
		//~ char k = cvWaitKey(5);
		
		//~ if( k == 127 ){
			//~ cvDestroyWindow("rgb");
			//~ cvDestroyWindow("depth");
			//~ break;
		//~ }
		//~ if( k == 8 ) {
			//~ std::ostringstream file;
			//~ file << filename << i_snap << suffix;
			//~ cv::imwrite(file.str(),rgbMat);
			//~ i_snap++;
		//~ }
		
		Kinect::marquer_obstacles(depthMat);	
	}
	
	//~ device.stopVideo();
	device.stopDepth();
	return 0;
}
