//Fichier Kinect.cpp
//Responsable de la communication avec la kinect et de l'affichage des données et de la grille
#include "Kinect.h"
#include "signal.h"
#include "AStarStatic.h"
#include <string.h>

//headers pour l´affichage
#include <gtk/gtk.h>
#include <cairo.h>
using namespace cv;
using namespace std;

//*****************************************************************************
//******************************PARTIE KINECT**********************************
//*****************************************************************************

//le code ci-dessous vient de libfreenect : les fonctions proposées par défaut sont utilisés pour s´interfacer avec cette bibliothèque
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

//contructeru de l´object Kinect
Kinect::Kinect(Grille* grille, positionBaseRoulante *pos){

    pos_ = pos;
    hauteurPersonne_= 2000;
    hauteurKinect_= 150;
    grille_= grille;
	die_=false;

	//paramètres de la kinect v1
    fx = 595.17079417;
    fy = 595.17079417;
    fxrgb = 525.70984318;
    fyrgb = 525.70984318;
    cu = 328.43206958;
    cvb = 251.7538;
    baseline = 0.07381;
    shift_offset = 1090.7479;
    shift_scale = 0.125;
}


#define TARGET 348 //la cible que l´on veut atteindre (pour la démo seulement)

//place les obstacles observés sur la grille
void Kinect::marquer_obstacles(Mat mask)
{
    int X = 640;
    int Y = 480;
    
    //remise a 0 de la grille sur les 480 premiers pixels : pour le forum seulement
    for(int x =0; x <480; x++)
	for(int y =0; y <480; y++)
		grille_-> grille_[x][y] = RAS;
    
    float xKinectbrut, yKinectbrut, zKinectbrut;
    int tailleGrille, grilleX, grilleY, grilleHauteur;
    for(int b=Y-1; b>=0; b--) // b = COLONNE SUR L'IMAGE MODIFIEE, EN PIXEL
    {
        for(int a = 0; a<X; a++) // a = LIGNE SUR L'IMAGE MODIFIEE, EN PIXEL
        {
            zKinectbrut = ((fx*baseline)/(shift_scale*(shift_offset-mask.at<u_int16_t>(b,a))))* 1000; //calcul de la profondeur en mm
            if(zKinectbrut > 0 && zKinectbrut < 2000)    
            {
                //calcul des coordonnees par rapport à la Kinect en mm
				xKinectbrut = (a-cu)/fx * zKinectbrut;
				yKinectbrut = (b-cvb)/fy * zKinectbrut;
				
				
				// Pour le forum : position du robot fixe a (240,0)
				tailleGrille = 5; //1px = 5mm
				grilleX = 240 - (a-cu)/fx * zKinectbrut / tailleGrille;
				grilleY = zKinectbrut / tailleGrille;
				grilleHauteur = -static_cast<int>(floor(yKinectbrut)) + hauteurKinect_;
				// filtrage vertical des points pour ne pas voir le sol comme un obstacle
				if (grilleHauteur<hauteurPersonne_ && grilleHauteur>0)
				{
					//placement des points sur la grille, avec hitbox
					if (grilleX < grille_->taille_ -1 && grilleX >= 1 && grilleY >= 1 && grilleY < grille_->taille_-1)
					{
						int tailleHitBox = 3;
						for (int i=grilleX-tailleHitBox; i<grilleX+tailleHitBox; i++)
							for (int j=grilleY-tailleHitBox; j<grilleY+tailleHitBox; j++)
								   if(grille_-> grille_[i][j] == 0)
										grille_-> grille_[i][j]=OBSTACLE + 1;
						//différentier obstacles / hitbox pour le forum
						grille_->grille_[grilleX][grilleY] = OBSTACLE;
					}
				}
			}
		}
    }   
}

void Kinect::killKinect() {
    die_ = true;
}

Grille *drawgrid;
Mat depthf(480,640,CV_8UC1);
pthread_mutex_t mut;	//mutex d´accés à la grille (entre thread dessin et thread kinect)
pthread_cond_t cond;

//boucle de traitement de la kinect
void* Kinect::runKinect(void* k) 
{
	//setup
    Kinect* kinect = static_cast<Kinect*>(k);
    drawgrid = kinect->grille_;
	Mat depthMat(480,640,CV_16UC1);
    Freenect::Freenect freenect;
    MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);
    device.startDepth();
    //grille ¨virtuelle¨ plus petite pour A* au forum (pour accélérer la recherche de trajectoire).
    char **sgrille = new char*[175];
    for(int i=0;i<175;i++)
            sgrille[i]=new char[175];
    clock_t time;
    while (!kinect->die_) 
    {
		//si pas d´image disponible : attente de 10ms puis relancer la boucle.
        if(!device.getDepth(depthMat))
		{
			delay(10);
			continue;
		}
		pthread_mutex_lock(&mut);
        depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
		time = clock();	//debug : utilisé pour cronométrer les boucles
		kinect->marquer_obstacles(depthMat); 
//	std::cout << "marquer" << (clock() - time) * 1000 / CLOCKS_PER_SEC << std::endl;
	
		 //executer AStar, division de grille par 2 pour aller plus vite
		for(int x =0; x <175; x++)
			for(int y =0; y <175; y++)
				sgrille[x][y] = kinect->grille_->grille_[2 *x][2 * y] + kinect->grille_->grille_[2 *x + 1][2 * y] + kinect->grille_->grille_[2 *x][2 * y + 1] + kinect->grille_->grille_[2 *x+1][2 * y+1];
			
		char *route = AStarStatic(240 / 2, 0, 240 / 2, TARGET / 2, sgrille, 175,175);
		char lroute[ 2 * strlen(route)];  
		for(int x =0; x <strlen(route); x++)
		{
			lroute[2 * x] = route[x];
			lroute[2 * x + 1] = route[x];
		}
		drawRouteOnMap(kinect->grille_->grille_, lroute, 240, 0);
		if(strlen(route) != 0)
			free(route);
		//	std::cout << (clock() - time) * 1000 / CLOCKS_PER_SEC << std::endl;

		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mut);
		delay(40);	//40ms sleep : laisse le temps pour dessiner la fenetre avant de reprendre le mutex
    }
    device.stopDepth();
    delete(sgrille);
	return 0;
}

//*****************************************************************************
//*****************************PARTIE AFFICHAGE********************************
//*****************************************************************************

// Pour le forum cette année, il était nécessaire d´aficher les images de la kinect.
// Nous avons utilisé GTK, tout simplement parce que l´un d´entre nous connaissait très bien cette bibliothèque.
// Mais a priori, si vous continuez d´utiliser QTCreator, il semble plus logique d´utiliser QT. 
// Cela dit, les deux sont équivalants pour ce que l´on veut faire en tout cas.

Mat im(480,640,CV_8UC3);	//image couleur de la kinect (définie ici pour ne pas être redéclarée à chaque itération).
clock_t fps;	//debug : horloge pour afficher le nombre d´images par secondes

//callback sur draw : cette fonction réalise le dessin proprement dit, à l´aide de la bilbiothèque cairo
gboolean redraw(GtkWidget *w)
{
	//creaton de la surface de dessin
    cairo_t *cr;
    cr = gdk_cairo_create(w->window);
    
    pthread_mutex_lock(&mut);
    pthread_cond_wait(&cond, &mut);
	
    
    //affichage de l'image de profondeur
    cvtColor(depthf, im, CV_GRAY2RGB);
    GdkPixbuf * pix = gdk_pixbuf_new_from_data(im.data, GDK_COLORSPACE_RGB, false, 8, 640, 480, 3*640, NULL, NULL);
    gdk_cairo_set_source_pixbuf(cr,pix,0,0);
    cairo_paint(cr);
    g_object_unref(pix);
   
   //creation de la grille affichagegrille a afficher
   guchar affichagegrille[480*480*3];
   for(int x = 0; x < 480; x++)	   
	for(int y = 0; y < 480; y++)
	    {
		if(drawgrid->grille_[x][y] == OBSTACLE +1) //obstacle
		{
			affichagegrille [480*3*y + 3*x] = 150;
			affichagegrille [480*3*y + 3*x +1] = 150;
			affichagegrille [480*3*y + 3*x + 2] = 150;
		}
		else if(drawgrid->grille_[x][y] == OBSTACLE) //obstacle
		{
			affichagegrille [480*3*y + 3*x] = 0;
			affichagegrille [480*3*y + 3*x +1] = 0;
			affichagegrille [480*3*y + 3*x + 2] = 0;
		}
		else if(drawgrid->grille_[x][y] == 'r')	//route
		{
			affichagegrille [480*3*y + 3*x] = 254;
			affichagegrille [480*3*y + 3*x +1] = 0;
			affichagegrille [480*3*y + 3*x + 2] = 0;
		}
		else if(y < 5 && x > 235+y && x < 245 - y)
		{
			affichagegrille [480*3*y + 3*x] = 0;
			affichagegrille [480*3*y + 3*x +1] = 254;
			affichagegrille [480*3*y + 3*x + 2] = 0;
		}
		else if(y > TARGET && y <TARGET + 6 && x > 237 && x < 243)
		{
			affichagegrille [480*3*y + 3*x] = 0;
			affichagegrille [480*3*y + 3*x +1] = 0;
			affichagegrille [480*3*y + 3*x + 2] = 254;
		}
		else
		{
			affichagegrille [480*3*y + 3*x] = 254;
			affichagegrille [480*3*y + 3*x +1] = 254;
			affichagegrille [480*3*y + 3*x + 2] = 254;
		}
			
	    }
	//affichage de la grille
    pix = gdk_pixbuf_new_from_data(affichagegrille, GDK_COLORSPACE_RGB, false, 8, 480, 480, 3*480, NULL, NULL);
    gdk_cairo_set_source_pixbuf(cr,pix,490,0);
    cairo_paint(cr);
    g_object_unref(pix);
    pthread_mutex_unlock(&mut);
    cairo_destroy(cr);
    //debug : fps
//	std::cout << "FPS " << (clock() - fps) * 1000 / CLOCKS_PER_SEC << std::endl;
	fps = clock();
	gtk_widget_queue_draw(w);
	return TRUE;
}

void* Kinect::displayRun(void*) {
    int argc = 0;
    //initialisation de gtk
    gtk_init(&argc, NULL);
    
    //creation de la fenetre et de son contenu
    GtkWidget *mainwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(mainwindow), "Vue du Roblind");
    gtk_window_set_deletable (GTK_WINDOW (mainwindow), FALSE);
    gtk_container_set_border_width (GTK_CONTAINER (mainwindow), 10);
    
    GtkWidget *table = gtk_table_new (2,2,FALSE);
    gtk_table_set_row_spacings (GTK_TABLE(table),10);
    gtk_table_set_col_spacings (GTK_TABLE(table),10);
    gtk_container_add(GTK_CONTAINER(mainwindow), table);
    
    GtkWidget *titre1 = gtk_label_new("Image de profondeur de la Kinect");
    GtkWidget *titre2 = gtk_label_new("Grille et trajectoire");
    gtk_table_attach_defaults (GTK_TABLE(table), titre1, 0,1,0,1);
    gtk_table_attach_defaults (GTK_TABLE(table), titre2, 1,2,0,1);
	
	GtkWidget *draw = gtk_drawing_area_new();
    gtk_drawing_area_size(GTK_DRAWING_AREA(draw), 640 + 10 + 480 + 2, 480 + 2);
    g_signal_connect(G_OBJECT(draw), "expose-event", G_CALLBACK(redraw), NULL);	//callback : a chaque fois que draw doit être redéssiné, redraw est appelée
    gtk_table_attach_defaults (GTK_TABLE(table), draw, 0,2,1,2);

    gtk_widget_queue_draw(draw);
    gtk_widget_show_all(mainwindow);
    
    //Remarque : gtk (comme qt et toutes les bibliothèque pour la création d´IHM) est de la programmation événementielle :
	//Après avoir configuré la fenêtre, on laisse le GtkMainEventLoop (gtk_main(), boucle infinie) la gérer, et traiter les signaux recus.
	//Pensez aux IHM java que vous avait faites en 1A : c´est pareil...
    gtk_main();
    return 0;
}
