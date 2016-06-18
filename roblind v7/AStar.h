#ifndef ASTAR_H
#define ASTAR_H
#include "roblind.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
#define n 200 	//taille de la grille
#define m 200  //taille de la grille
#define INFTY 100000
#define OPEN 1
#define CLOSE 0
#define dir 8
#define X 0
#define Y 1
#define RESOLUTION 5
#define CM_TO_PAS_CODEUR 1

using namespace std;

typedef struct
{
    int x;
    int y;
    float gscore;
    float fscore;
}node;



class AStar
{
public:
    CRoblind *m_robot;
    double dirToDegree[8];
    int dx[dir];
    int dy[dir];
    int cameFrom[n][m][2];
    node set[2][100000];
    int indice[2];
     vector< vector<int> > path;
    //destination
    int goalX, goalY;
    int map[n][m]; //la carte utilisee par AStar


    AStar(CRoblind *robot);
    string AStarAlgo(int startx, int starty, int endx, int endy); //algorithme A* : trouve un chemin de start a end, sur la carte map
    float distance (int startx, int starty, int endx, int endy);
    int getCurrent(int s);
    void removeSet(int s, int j);
    int inSet(int s, int x, int y);
    double rotation(string s);
    double translation(string s);
    void parse_path(int xA, int yA, int xB, int yB);
    string reconstruct_path(int startx, int starty, int currx, int curry);
    void displayMap();// display the map with the route

    //try to jump from x to y, to go from start to end
    //return x + sizeof(map) * y
    int jump(int x, int y, int direction);
    void generateRandomGrid(int randseed, int N);












};
#endif // ASTAR_H
