
#include "AStar.h"

AStar::AStar(CRoblind *robot)
{
    m_robot=robot;
   dirToDegree[0] = 0;
    dirToDegree[1] = 45;
    dirToDegree[2] = 90;
    dirToDegree[3] = 135;
    dirToDegree[4] = 180;
    dirToDegree[5] = 225;
    dirToDegree[6] = 270;
    dirToDegree[7] = 315;

    dx[0] = 1;
    dx[1] = 1;
    dx[2] = 0;
    dx[3] = -1;
    dx[4] = -1;
    dx[5] = -1;
    dx[6] = 0;
    dx[7] = 1;

    dy[0] = 0;
    dy[1] = 1;
    dy[2] = 1;
    dy[3] = 1;
    dy[4] = 0;
    dy[5] = -1;
    dy[6] = -1;
    dy[7] = -1;
}

float AStar::distance (int startx, int starty, int endx, int endy)
{
	//~ return abs(startx - endx) + abs(starty - endy);
	return sqrt((startx - endx)*(startx - endx)+(starty - endy)*(starty - endy));
}

int AStar::getCurrent(int s)
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

void AStar::removeSet(int s, int j)
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

int AStar::inSet(int s, int x, int y)
{
    for (int i = 0; i < indice[s]; i++)
        if(set[s][i].x == x && set[s][i].y == y)
            return i;
    return -1;
}

double AStar::rotation(string s) {
    unsigned int pos = 0;
    double sum = 0.0;
    while (pos < s.length())
    {
        sum += dirToDegree[s[pos] - '0'];
        pos += 1;
    }
    cout << "   ROTATION : " << sum/s.length() << "\n";
    return sum/s.length();
}

double AStar::translation(string s)
{
    unsigned int pos = 0;
    int x = 0;
    int y = 0;
    while (pos < s.length())
    {
        switch(s[pos] - '0')
        {
            case 0: {x += 1; break;}
            case 1: {x += 1; y -= 1; break;}
            case 2: {y -= 1; break;}
            case 3: {x -= 1; y -= 1; break;}
            case 4: {x -= 1; break;}
            case 5: {x -= 1; y += 1; break;}
            case 6: {y += 1; break;}
            case 7: {x += 1; y += 1; break;}
        }
        pos += 1;
    }
    cout << "   TRANSLATION : " << (sqrt(x*x + y*y)) << " -- x" << x << " -y" << y << "\n"; // DEBUG
    return (sqrt(x*x + y*y));
}

void AStar::parse_path(int xA, int yA, int xB, int yB) {
        string s = AStarAlgo(xA, yA, xB, yB);
        if (s.length() > 0)
        {
            double current_o = m_robot->m_baseRoulante->m_posBR.alpha;
            unsigned int prevPos = 0;
            unsigned int pos = 0;
            int last1 = -1; // last char analysed
            int motif = 0;
            string dirOffset = "";
            int cpt = 0; // TODO REMOVE
            while (pos < s.length())
            {
                if (pos == prevPos)
                {
                    last1 = s[pos] - '0';
                    pos += 1;
                }
                else if (pos == prevPos + 1)
                {
                    cout << "DEBUG-1\n" ; // DEBUG
                    int i = s[pos] - '0';
                    if (i == last1)
                    {
                        while (pos < s.length() && s[pos] - '0' == i)
                        {
                            pos += 1;
                        }
                    }
                    else // i != last1 -- Cas particulier de motif
                    {
                        /*
                        if (pos + 1 < s.length() && s[pos+1] == i)
                        {
                            while (pos + 1 < s.length() && s[pos+1] == i)
                            {
                                // MOTIF TYPE ab*
                            }
                        }
                        */

                        while (pos + 2 < s.length() && s[pos+1] - '0' == last1 && s[pos+2] - '0' == i)
                        {
                            pos += 2;
                        }
                        if (pos + 1 < s.length() && s[pos+1] == last1) pos += 1 ;
                        if (pos - prevPos == 1 && dirOffset.length() == 0)
                            dirOffset = s[pos];
                        else
                        {
                            // ADD ROTATION TO QUEUE
                            path.push_back(vector<int> (1, static_cast<int>(rotation(dirOffset + s.substr(prevPos, pos - prevPos)) - current_o)));
                            // ADD TRANSLATION TO QUEUE
                            path.push_back(vector<int> (0, static_cast<int>(RESOLUTION*translation(dirOffset + s.substr(prevPos, pos - prevPos))*CM_TO_PAS_CODEUR)));
                            dirOffset = "";
                        }
                        pos += 1;
                        prevPos = pos;
                    }
                }
                else if (motif > 0)
                {
                    cout << "DEBUG-2\n" ; // DEBUG
                    bool test = true;
                    int len = pos - prevPos;
                    while (test && pos + len < s.length())
                    {
                        for (int i = 0; i < len; i++)
                        {
                            if (s[pos+i] != s[prevPos+i]) test = false;
                        }
                        if (test)
                        {
                            pos += len;
                            motif += 1;
                        }
                    }

                    // ADD ROTATION TO QUEUE
                    path.push_back(vector<int> (1, static_cast<int>(rotation(dirOffset + s.substr(prevPos, len * motif)) - current_o)));
                    // ADD TRANSLATION TO QUEUE
                    path.push_back(vector<int> (0, static_cast<int>(RESOLUTION*translation(dirOffset + s.substr(prevPos, len * motif))*CM_TO_PAS_CODEUR)));
                    dirOffset = "";

                    motif = 0 ;
                }
                else
                {
                    cout << "DEBUG-3\n" ; // DEBUG
                    int i = s[pos-1] - '0';
                    unsigned int tempPos = pos;
                    while (tempPos - pos < pos - prevPos - 1 && tempPos + 1 < s.length() && s[tempPos + 1] == i)
                    {
                        tempPos += 1 ;
                    }
                    if (tempPos + 1 < s.length() && s[tempPos+1] == s[pos])
                    {
                        if (tempPos - pos == pos - prevPos - 1)
                            motif = 1;
                        else
                        {
                            // ADD ROTATION TO QUEUE
                            path.push_back(vector<int> (1, static_cast<int>(rotation(dirOffset + s.substr(prevPos, 2*pos - prevPos - tempPos)) - current_o)));
                            // ADD TRANSLATION TO QUEUE
                            path.push_back(vector<int> (0, static_cast<int>(RESOLUTION*translation(dirOffset + s.substr(prevPos, 2*pos - prevPos - tempPos))*CM_TO_PAS_CODEUR)));
                            dirOffset = "";

                            prevPos = 2*pos - tempPos ;
                            if (pos - prevPos > 1)
                                motif = 1 ;
                        }
                    }
                    else
                    {
                        // ADD ROTATION TO QUEUE
                        path.push_back(vector<int> (1, static_cast<int>(rotation(dirOffset + s.substr(prevPos, pos - prevPos + 1)) - current_o)));
                        // ADD TRANSLATION TO QUEUE
                        path.push_back(vector<int> (0, static_cast<int>(RESOLUTION*translation(dirOffset + s.substr(prevPos, pos - prevPos + 1))*CM_TO_PAS_CODEUR)));
                        dirOffset = "";

                        pos += 1;
                        prevPos = pos;
                    }
                }
                if (cpt < 100) {
                    cout << pos << " ";
                    cpt += 1;
                }
            }

            if (pos != prevPos) {
                // ADD FINAL ROTATION TO QUEUE
                path.push_back(vector<int> (1, static_cast<int>(rotation(dirOffset + s.substr(prevPos, pos - prevPos)) - current_o)));
                // ADD FINAL TRANSLATION TO QUEUE
                path.push_back(vector<int> (0, static_cast<int>(RESOLUTION*translation(dirOffset + s.substr(prevPos, pos - prevPos))*CM_TO_PAS_CODEUR)));
                dirOffset = "";
            }
        }
}

string AStar::reconstruct_path(int startx, int starty, int currx, int curry)
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

void AStar::displayMap()// display the map with the route
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
int AStar::jump(int x, int y, int direction)
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

string AStar::AStarAlgo(int startx, int starty, int endx, int endy)
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

void AStar::generateRandomGrid(int randseed, int N)
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




