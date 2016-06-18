// AStartStatic.cpp
// algorithme A* avec Jump Point Search, en C
// Une ré-écriture en C++ (en gros, l´intégration de l´algorithme dans une classe) avait été commencée dans AStar.cpp, mais n´était pas fonctionnelle.
// Ce fichier contient donc l´algorithme de base, appelé directement : on lui donne en paramètre une grille, avec point de départ et point d´arrivée.

// A priori, ce fichier n´est plus nécessaire si l´intégration de AStar dans le reste du code est faite


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#define INFTY 100000

#define OPEN 1
#define CLOSE 0

#define dir 8
#define X 0
#define Y 1

//taille limite de la grille
#define MAXSIZE 1000

//definition des variables pour A*
int dx[dir]={1, 1, 0, -1, -1, -1,  0,  1};
int dy[dir]={0, 1, 1,  1,  0, -1, -1, -1};
typedef struct {
	int x;
	int y;
	float gscore;
	float fscore;
}node;
int n,m;
static int cameFrom[MAXSIZE][MAXSIZE][2];	
static node set[2][100000];
static int indice[2];
//destination
static int goalX, goalY;


//utility functions : a ne pas appeler directement (i.e. privés)
float distance (int startx, int starty, int endx, int endy)
{
	//~ return abs(startx - endx) + abs(starty - endy);
	return sqrt((startx - endx)*(startx - endx)+(starty - endy)*(starty - endy));
}

int getCurrent(int s)
{
	int j = 0;
	int min = INFTY;
	for(int x = 0; x < indice[s]; x++)
		if(set[s][x].fscore < min)
		{
			min = set[s][x].fscore;
			j = x;
		}
	return j;
}

void removeSet(int s, int j)
{
	for (int a = j; a < indice[s]-1; a++)
	{
		set[s][a].x = set[s][a+1].x;
		set[s][a].y = set[s][a+1].y;
		set[s][a].fscore = set[s][a+1].fscore;
		set[s][a].gscore = set[s][a+1].gscore;
	}
	indice[s]--;
}

int inSet(int s, int x, int y)
{
	for (int i = 0; i < indice[s]; i++)
		if(set[s][i].x == x && set[s][i].y == y)
			return i;
	return -1;
}

//a partir de la grille de A*, ré-écrit le chemin, sour la forme d´une chaine de caractères
char *reconstruct_path(int startx, int starty, int currx, int curry)
{
	char *path = (char *)malloc(1000 * sizeof(char));
	int ind = 0;
	while(currx != startx || curry != starty)
	{
		int parx = cameFrom[currx][curry][X];
		int pary = cameFrom[currx][curry][Y];
		int c = 0;
		if(parx == currx)
			if(pary < curry)
				c =6;
			else
				c =2;
		else if(pary == curry)
			if(parx < currx)
				c =4;
			else
				c =0;
		else if(parx < currx)
			if(pary < curry)
				c =5;
			else
				c=3;
		else
			if(pary < curry)
				c =7;
			else
				c=1;
		while( currx != parx || curry != pary)
		{				
			path[ind] ='0' + ((c + dir/2 )%dir);
			currx+=dx[c];
			curry+=dy[c];
			ind ++;
		}
	}
	//flip path around
	char *secondpath =(char *)malloc((ind +1) * sizeof(char));
	for(int x = 0; x < ind; x++)
		secondpath[x] = path[ind-1-x];
	secondpath[ind] = '\0';	
	free(path);
	return secondpath; 
}

//implémente le saut de Jump Point Search
int jump(int x, int y, int direction, char**map)
{
	int nx = x + dx[direction];
	int ny = y + dy[direction];
	if(nx < 0 || nx > n -1 || ny < 0 || ny > m -1 || map[nx][ny] != 0)
		return -1;
	
	if( nx == goalX && ny == goalY)
		return nx + m * ny;
		
	//look for forced neighboors
	int x2 = nx +  dx[direction];
	int y2 = ny +  dy[direction];
	//straight move
	if(direction % 2 == 0)
	{
		//find cells above / below : if blocked, it is forced
		if(nx + dy[direction] > 0 && nx + dy[direction] < n && ny + dx[direction] > -1 && ny + dx[direction] < m && x2 + dy[direction] > 0 && x2 + dy[direction] < n && y2 + dx[direction] > -1 && y2 + dx[direction] < m)
			if(map[nx + dy[direction]][ny + dx[direction]] != 0 && map[x2 + dy[direction]][y2 + dx[direction]] == 0)
				return nx  + m * ny;
				
				
		if(nx - dy[direction] > 0 && nx - dy[direction] < n && ny - dx[direction] > -1 && ny - dx[direction] < m && x2 - dy[direction] > 0 && x2 - dy[direction] < n && y2 - dx[direction] > -1 && y2 - dx[direction] < m)
			if(map[nx - dy[direction]][ny - dx[direction]] != 0 && map[x2 - dy[direction]][y2 - dx[direction]] == 0)
				return nx  + m * ny;
	}
	else
	{
		//diagonal : look in corners
		if(y2  > -1 && y2 < m)
			if(map[x][ny] != 0 && map[x][y2] == 0)
				return nx  + m * ny;
		
		if(x2  > -1 && x2 < n)
			if(map[nx][y] != 0 && map[x2][y] == 0)
				return nx  + m * ny;	
		//perform horizontal and vertical check
		if(jump(nx,ny,(direction+1)%dir, map) != -1)
			return nx + ny * m;
		if(jump(nx,ny,(direction-1)%dir, map) != -1)
			return nx + ny * m;
		
	}
	return jump(nx, ny, direction,map);
}

//Algorithme A* proprement dit.
//Cette fonction est la seule qui doit être accessible à l´utilisateur pour faire tourner A*
char *AStarStatic(int startx, int starty, int endx, int endy, char **map, int sizex, int sizey)
{
	//setup
	goalX = endx;
	goalY = endy;
	n = sizex;
	m = sizey;
	if(n > MAXSIZE || m > MAXSIZE)
		return "";
	if(map[endx][endy] != 0)
	{
		printf("no path\n");
		return "";
	}
    indice[OPEN] = 1;
    indice[CLOSE] = 0;
    set[OPEN][0].x = startx;
    set[OPEN][0].y = starty;
    set[OPEN][0].fscore =  distance(startx, starty, endx, endy);
    set[OPEN][0].gscore = 0;
    
    int j, currx, curry, nx, ny, pos;
    float currg, newg, newf;
    while(indice[OPEN] != 0)
    {
		//find current node
		j = getCurrent(OPEN);
		currx = set[OPEN][j].x;
		curry = set[OPEN][j].y;
		currg = set[OPEN][j].gscore;
		
		//put node in close list
		set[CLOSE][indice[CLOSE]].x = currx;
		set[CLOSE][indice[CLOSE]].y = curry;
		set[CLOSE][indice[CLOSE]].fscore = set[OPEN][j].fscore;
		indice[CLOSE]++;
		removeSet(OPEN, j);
		//look for neighboor
		for(int i=0; i<8; i++)
		{
			//jump
			j = jump(currx, curry, i, map);
			nx = j % m;
			ny = j / m;
			if(nx == -1)
				continue;
			if(nx == endx && ny == endy)
			{
				cameFrom[nx][ny][X] = currx;
				cameFrom[nx][ny][Y] = curry;
				return reconstruct_path(startx, starty, nx, ny);
			}					
			newg = currg +  distance(nx, ny, currx, curry);
			newf = newg  +  distance(nx, ny, endx, endy);
			pos =  inSet(CLOSE, nx, ny);
			if(pos != -1 && set[CLOSE][pos].fscore < newf)
					continue;
			pos = inSet(OPEN, nx, ny);
			if( pos!= -1 &&  set[OPEN][pos].fscore < newf)
					continue;
			if( pos == -1)
			{
				set[OPEN][indice[OPEN]].x = nx;
				set[OPEN][indice[OPEN]].y = ny;
				set[OPEN][indice[OPEN]].gscore = newg;
				set[OPEN][indice[OPEN]].fscore = newf;
				indice[OPEN]++;	
			}
			else
			{
				set[OPEN][pos].gscore = newg;
				set[OPEN][pos].fscore = newf;
			}
			cameFrom[nx][ny][X] = currx;
			cameFrom[nx][ny][Y] = curry;
		}
	}
	printf("no path\n");
	return "";
}

//Pour le forum : dessine la route sur la grille map (c´est comme cela qu´on affichait la grille à l´écran).
void drawRouteOnMap(char **map, char *route, int xA, int yA)
{
	int length = strlen(route);
	if(length>0)
	{
		int j; 
		char c;
		int x=xA;
		int y=yA;
		for(int i=0;i<length;i++)
		{
			c =route[i];
			j=c - '0';
			x=x+dx[j];
			y=y+dy[j];
			map[x][y]='r';
		}
	}
}
