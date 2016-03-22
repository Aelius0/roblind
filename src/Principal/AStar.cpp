/*
 * AStar.c
 * 
 * Copyright 2016 Matthieu <matthieu@Matthieu-M4500>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "AStar.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>

using namespace std;

#define INFTY 100000

#define OPEN 1
#define CLOSE 0

#define dir 8
#define X 0
#define Y 1

int dx[dir]={1, 1, 0, -1, -1, -1,  0,  1};
int dy[dir]={0, 1, 1,  1,  0, -1, -1, -1};


typedef struct {
	int x;
	int y;
	float gscore;
	float fscore;
}node;



static int cameFrom[n][m][2];
static node set[2][1000000];
static int indice[2];
//destination
static int goalX, goalY;

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

string reconstruct_path(int startx, int starty, int currx, int curry)
{
	string path = "";
	int ind = 0;
	while(currx != startx || curry != starty)
	{
		int parx = cameFrom[currx][curry][X];
		int pary = cameFrom[currx][curry][Y];
		
		//~ printf("%d %d %d %d\n", currx, curry, parx, pary);
		//get the direction caracter
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
			char car = '0' + ((c + dir/2 )%dir);
			path = car + path;
			currx+=dx[c];
			curry+=dy[c];
			ind ++;
		}
	}
	return path; 
}

void displayMap()// display the map with the route
{
	//~ for(int x = 0; x < indice[OPEN]; x++)
		//~ map[set[OPEN][x].x][set[OPEN][x].y] = 5;
	for(int y=0;y<m;y++)
	{
		for(int x=0;x<n;x++)
			if(map[x][y]==0)
				printf(".");
			else if(map[x][y]==1)
				printf("O"); //obstacle
			else if(map[x][y]==2)
				printf("S"); //start
			else if(map[x][y]==3)
				printf("R"); //route
			else if(map[x][y]==4)
				printf("F"); //finish
			else if(map[x][y] == 5)
				printf("o");
			else if(map[x][y] == 6)
				printf("v");
			else if(map[x][y] == 7)
				printf("c");
		printf("\n");
	}
}

//try to jump from x to y, to go from start to end 
//return x + sizeof(map) * y
int jump(int x, int y, int direction)
{
	int nx = x + dx[direction];
	int ny = y + dy[direction];
	//~ if( x == 11)
		//~ printf("%d %d\n", nx, ny);
	if(nx < 0 || nx > n -1 || ny < 0 || ny > m -1 || map[nx][ny] == 1)
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
			if(map[nx + dy[direction]][ny + dx[direction]] == 1 && map[x2 + dy[direction]][y2 + dx[direction]] != 1)
				return nx  + m * ny;
				
				
		if(nx - dy[direction] > 0 && nx - dy[direction] < n && ny - dx[direction] > -1 && ny - dx[direction] < m && x2 - dy[direction] > 0 && x2 - dy[direction] < n && y2 - dx[direction] > -1 && y2 - dx[direction] < m)
			if(map[nx - dy[direction]][ny - dx[direction]] == 1 && map[x2 - dy[direction]][y2 - dx[direction]] != 1)
				return nx  + m * ny;
	}
	else
	{
		//diagonal : look in corners
		if(y2  > -1 && y2 < m)
			if(map[x][ny] == 1 && map[x][y2] != 1)
				return nx  + m * ny;
		
		if(x2  > -1 && x2 < n)
			if(map[nx][y] == 1 && map[x2][y] != 1)
				return nx  + m * ny;	
		//perform horizontal and vertical check
		if(jump(nx,ny,(direction+1)%dir) != -1)
			return nx + ny * m;
		if(jump(nx,ny,(direction-1)%dir) != -1)
			return nx + ny * m;
		
	}
	return jump(nx, ny, direction);
}

string AStar(int startx, int starty, int endx, int endy)
{
	//setup
	goalX = endx;
	goalY = endy;
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
			j = jump(currx, curry, i);
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
	return "";
}

void generateRandomGrid(int randseed, int N)
{
	srand(randseed);
	
	//generation de N obstacles, de taille aleatoire, sauf dans les 5 premieres / dernieres lignes du plateau
	
	for(int i = 0; i<N; i++)
	{
		int posx = rand() % (n - 10);
		int posy = rand() % m + 5;
		int sizex = rand() % 40;
		int sizey = rand() % 20;
		
		for(int x = posx; x < posx + sizex; x++)
			for(int y = posy; y < posy + sizey; y++)
				if(x < n && y < m - 5)
					map[x][y] = 1;
		
	}
}



int main(int argc, char **argv)
{
	
	//setup map
	generateRandomGrid(time(NULL), 120);

	int xA, yA, xB, yB;
	xA = n / 2;
	yA = 0;
	xB = n/2 - 20 + rand() % 40;
	yB = m - 1;

		
	clock_t start = clock();
	string route = AStar(xA, yA, xB, yB);
	clock_t end = clock();
	float time_elapsed = (float)(end - start)* 1000.0 / CLOCKS_PER_SEC;
	//~ printf("Time to calculate : %f\n", time_elapsed);
	printf("%f\n", time_elapsed);
	cout << route << endl;
	
	// follow the route on the map and display it
	int length = route.length();
	if(length>0)
	{
		int j; char c;
		int x=xA;
		int y=yA;
		map[x][y]=2;
		for(int i=0;i<length;i++)
		{
			c =route[i];
			j=atoi(&c);
			x=x+dx[j];
			y=y+dy[j];
			map[x][y]=3;
		}
		map[x][y]=4;

	}
	displayMap();
	
	return 0;
}

