#ifndef ASTAR_H
#define ASTAR_H
	
	//taille de la grille
	#define n 200
	#define m 200
	#include <string>

	std::string AStar(int startx, int starty, int endx, int endy); //algorithme A* : trouve un chemin de start a end, sur la carte map
	int map[n][m]; //la carte utilisee par AStar
	
#endif // ASTAR_H
