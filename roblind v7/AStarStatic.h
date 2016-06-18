#ifndef ASTARS_H
#define ASTARS_H
	
	char *AStarStatic(int startx, int starty, int endx, int endy, char **mapData, int sizex, int sizey);
	void drawRouteOnMap(char **map, char *route, int xA, int yA);
	
#endif // ASTAR_H
