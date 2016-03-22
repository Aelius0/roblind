#include <iostream>
#include <cmath>
#include <iomanip>


enum nature_sol : int {
    RAS= 0,
    OBSTACLE=1,
    MUR=2,
    CHAUSSEE=3
};

struct incorrectCoordonnees{};

class Grille{

private:
    double resolutionx_; // en m
    double resolutiony_; // en m
    int taillex_;
    int tailley_;
public:
    int** grille_;

    Grille(double resolutionx,double resolutiony,int taillex,int tailley);
    Grille(int resolution, int taille);
    ~Grille();
    void afficher();
    void translaterRepereGrille(int translationx, int translationy);
    void set(int i, int j, nature_sol type);
    int get(int i, int j);
    double getResolutionX(){return resolutionx_;}
    double getResolutionY(){return resolutiony_;}
    int getTailleX(){return taillex_;}
    int getTailleY(){return tailley_;}

};
