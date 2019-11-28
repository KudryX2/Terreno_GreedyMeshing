#include "World.h"

#include <stdlib.h>
#include <time.h>

using namespace std;

World::World(){

	cout << "\nGenerating terrain";
	generateTerrain();

	cout << "\nCreating chunks";
	createChunks();

	cout << "\nGenerating structures";
	generateStructures();

	freeMemory();
}

void World::createChunks(){
//	cout << "\nCreating chunks";
	// CHUNKS MATRIX
	chunks = new Chunk***[SIZE];	
	
	for(int i = 0 ; i < SIZE ; ++i){
		chunks[i] = new Chunk**[HEIGHT];
		for(int j = 0 ; j < HEIGHT ; ++j){
			chunks[i][j] = new Chunk*[SIZE];
		}
	}
	
	float stime = glutGet(GLUT_ELAPSED_TIME);
	int ammount = 0;
	int py = CHUNKSIZE*CUBESIZE/2;
//	int py =  0;
	int px = -CHUNKSIZE*CUBESIZE*SIZE/2 + CHUNKSIZE*CUBESIZE/2;
	int pz = -CHUNKSIZE*CUBESIZE*SIZE/2 + CHUNKSIZE*CUBESIZE/2;

	int ipy = py;
	int ipx = px;
	int ipz = pz;

	for(int y = 0 ; y < HEIGHT ; ++y){
		for(int x = 0 ; x < SIZE ; ++x){
			for(int z = 0 ; z < SIZE ; ++z){
				chunks[x][y][z] = new Chunk(px,py,pz);
			//	if(y == 0) 
					chunks[x][y][z]->fillChunk(terrainHeightMap,x,z);
				pz+=CHUNKSIZE*CUBESIZE;
				++ammount;
			}
			pz = ipz;
			px+=CHUNKSIZE*CUBESIZE;
		}
		py+=CHUNKSIZE*CUBESIZE;
		px = ipx;
		pz = ipz;	
	}

//	cout << "\nLinking chunks";
	for(int y = 0 ; y < HEIGHT ; ++y){
		for(int x = 0 ; x < SIZE ; ++x){
			for(int z = 0 ; z < SIZE ; ++z){
				if(y < HEIGHT-1) chunks[x][y][z]->yPosCh = chunks[x][y+1][z]; else chunks[x][y][z]->yPosCh = NULL;
				if(y > 0) 	     chunks[x][y][z]->yNegCh = chunks[x][y-1][z]; else chunks[x][y][z]->yNegCh = NULL;

				if(x < SIZE-1)	 chunks[x][y][z]->xPosCh = chunks[x+1][y][z]; else chunks[x][y][z]->xPosCh = NULL;
				if(x > 0)		 chunks[x][y][z]->xNegCh = chunks[x-1][y][z]; else chunks[x][y][z]->xNegCh = NULL;

				if(z < SIZE-1) 	 chunks[x][y][z]->zPosCh = chunks[x][y][z+1]; else chunks[x][y][z]->zPosCh = NULL;
				if(z > 0)		 chunks[x][y][z]->zNegCh = chunks[x][y][z-1]; else chunks[x][y][z]->zNegCh = NULL;
			}
		}
	}

//	cout << "\nOptimizing chunks";
	for(int y = 0 ; y < HEIGHT ; ++y)
		for(int x = 0 ; x < SIZE ; ++x)
			for(int z = 0 ; z < SIZE ; ++z)
				chunks[x][y][z]->updateChunk(false);
		//	cout << " Loading: " << (float)current/total*100  << " %" << endl;


//	cout << "\nFind interactiveChunks";  
// 	MEJORAR (ahorrar bucles, tener en cuenta la posicion inicial)
	int dist;
	for(int y = 0 ; y < HEIGHT ; ++y){
		for(int x = 0 ; x < SIZE ; ++x){
			for(int z = 0 ; z < SIZE ; ++z){
				dist = pow((pow(chunks[x][y][z]->x,2)+pow(chunks[x][y][z]->y,2)+pow(chunks[x][y][z]->z,2)),(float)1/3); 
				if(dist < 150) interactiveChunks.push_back(chunks[x][y][z]);
			}
		}
	}	

/* */	
	cout << " created: " << ammount << " chunks | in " << glutGet(GLUT_ELAPSED_TIME) - stime << " mils";
/* */
}

void World::generateTerrain(){
	//Create terrain
	terrainHeightMap = new int*[SIZE*CHUNKSIZE];
	for(int i = 0 ; i < SIZE*CHUNKSIZE ; ++i)
		terrainHeightMap[i] = new int[SIZE*CHUNKSIZE];

	//Init terrain
	int h = 500;
	for(int i = 0 ; i < SIZE*CHUNKSIZE ; ++i){
		for(int j = 0 ; j < SIZE*CHUNKSIZE ; ++j){
			terrainHeightMap[i][j] = h;
		}
	//	if(i%2 != 0)h+=100;
	}

	//Create mountains
//	elevateTerrain(-20, -20, 20, 6);

	int ex,ez;

	srand (time(NULL));
	
	for(int i = 0 ; i < 100 ; ++i){
		ex = rand()%(SIZE*CHUNKSIZE/2-20);
		ez = rand()%(SIZE*CHUNKSIZE/2-20);

		elevateTerrain(ex,ez,20,3);
	}


}

void World::elevateTerrain(int x, int z, int size, int height){
	int sx = x+SIZE*CHUNKSIZE/2 ;
	int sz = z+SIZE*CHUNKSIZE/2 ;

	int radius = size/2;
	int radiusDec = radius/height;

	int dist;

	for(int y = 0 ; y < height ; ++y){
		for(int i = sx - size/2 ; i < sx + size/2 ; ++i){
			for(int j = sz - size/2 ; j < sz + size/2 ; ++j){
				
				dist = pow((pow(i - sx,2)+pow(j - sz,2)),(float)1/2); 
				if( dist < radius)	terrainHeightMap[i][j]+=100;
			}	
		}
		radius-=radiusDec;
	}

}

void World::generateStructures(){
//	placeCube(50,650,50,true,false);
}

Chunk* World::getCloserChunk(int x, int y, int z){
	float dist = pow((pow(chunks[0][0][0]->x-x,2)+pow(chunks[0][0][0]->y-y,2)+pow(chunks[0][0][0]->z-z,2)),(float)1/3);
	float disti;	
	Chunk* chunkToReturn = chunks[0][0][0];

	for(int yi = 0 ; yi < HEIGHT ; ++yi){
		for(int xi = 0 ; xi < SIZE ; ++xi){
			for(int zi = 0 ; zi < SIZE ; ++zi){
				disti = pow((pow(x-chunks[xi][yi][zi]->x,2)+pow(y-chunks[xi][yi][zi]->y,2)+pow(z-chunks[xi][yi][zi]->z,2)),(float)1/3);
				if(disti < dist){
					dist = disti;
					chunkToReturn = chunks[xi][yi][zi];
				}	
			}
		}
	}	
	return chunkToReturn;
}

Chunk* World::getCloserChunki(int x, int y, int z){
	float dist = pow((pow(interactiveChunks[0]->x-x,2)+pow(interactiveChunks[0]->y-y,2)+pow(interactiveChunks[0]->z-z,2)),(float)1/3);
	float disti;

	Chunk* chunkToReturn = interactiveChunks[0];

	for(int i = 0 ; i < interactiveChunks.size() ; ++i){
		disti = pow((pow(x-interactiveChunks[i]->x,2)+pow(y-interactiveChunks[i]->y,2)+pow(z-interactiveChunks[i]->z,2)),(float)1/3);
		if(disti < dist){
			dist = disti;
			chunkToReturn = interactiveChunks[i];
		}	
	}
	return chunkToReturn;
}

void World::destroyCube(Camera* camera, bool info){

	for( int offset = 0 ; offset < 500 ; ++offset){
		int x = camera->posX+(sin(camera->rotY*3.14/180)*offset);
		int y = camera->posY+(tan(-camera->rotX*3.14/180)*offset);
		int z = camera->posZ+(cos(camera->rotY*3.14/180)*offset);

		Chunk* chunk = getCloserChunki(x,y,z);

		int posx = chunk->x-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;
		int posy = chunk->y-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;
		int posz = chunk->z-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;

		for(int yi = 0 ; yi < CHUNKSIZE ; ++yi){
			for(int zi = 0 ; zi < CHUNKSIZE ; ++zi){
				for(int xi = 0 ; xi < CHUNKSIZE ; ++xi){
					if((chunk->cubes[xi][yi][zi]==1)&&(y+CUBESIZE/2 > posy && y-CUBESIZE/2 < posy)&&(x+CUBESIZE/2 > posx && x-CUBESIZE/2 < posx)&&(z+CUBESIZE/2 > posz && z-CUBESIZE/2 < posz)){	
						chunk->removeCube(xi,yi,zi,true,info);
						return;				
					}
					posx+=CUBESIZE;
				}
				posx = chunk->x - CHUNKSIZE/2 * CUBESIZE+CUBESIZE/2;
				posz+=CUBESIZE;
			}
			posz = chunk->z - CHUNKSIZE/2 * CUBESIZE + CUBESIZE/2;
			posy +=CUBESIZE;			
		}
	}
	
}


void World::placeCube(Camera* camera, bool info){

	for( int offset = 50 ; offset < 500 ; ++offset){
		int x = camera->posX+(sin(camera->rotY*3.14/180)*offset);
		int y = camera->posY+(tan(-camera->rotX*3.14/180)*offset);
		int z = camera->posZ+(cos(camera->rotY*3.14/180)*offset);

		Chunk* chunk = getCloserChunki(x,y,z);

		int posx = chunk->x-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;
		int posy = chunk->y-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;
		int posz = chunk->z-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;

		for(int yi = 0 ; yi < CHUNKSIZE ; ++yi){
			for(int zi = 0 ; zi < CHUNKSIZE ; ++zi){
				for(int xi = 0 ; xi < CHUNKSIZE ; ++xi){
					if((chunk->cubes[xi][yi][zi]==0)&&(y+CUBESIZE/2 > posy && y-CUBESIZE/2 < posy)&&(x+CUBESIZE/2 > posx && x-CUBESIZE/2 < posx)&&(z+CUBESIZE/2 > posz && z-CUBESIZE/2 < posz)&&
						(
						(xi < CHUNKSIZE-1 && chunk->cubes[xi+1][yi][zi]==1 || xi > 0 && chunk->cubes[xi-1][yi][zi]==1)||
						(zi < CHUNKSIZE-1 && chunk->cubes[xi][yi][zi+1]==1 || zi > 0 && chunk->cubes[xi][yi][zi-1]==1)||
						(yi < CHUNKSIZE-1 && chunk->cubes[xi][yi+1][zi]==1 || yi > 0 && chunk->cubes[xi][yi-1][zi]==1)
						|| 
						(chunk->xPosCh != NULL && xi == CHUNKSIZE-1 && chunk->xPosCh->cubes[0][yi][zi]==1)||
						(chunk->xNegCh != NULL && xi == 0 && chunk->xNegCh->cubes[CHUNKSIZE-1][yi][zi]==1)||
						(chunk->zPosCh != NULL && zi == CHUNKSIZE-1 && chunk->zPosCh->cubes[xi][yi][0]==1)||
						(chunk->zNegCh != NULL && zi == 0 && chunk->zNegCh->cubes[xi][yi][CHUNKSIZE-1]==1)||
						(chunk->yPosCh != NULL && yi == CHUNKSIZE-1 && chunk->yPosCh->cubes[xi][0][zi]==1)||
						(chunk->yNegCh != NULL && yi == 0 && chunk->yNegCh->cubes[xi][CHUNKSIZE-1][zi]==1)
						)){	
						chunk->placeCube(xi,yi,zi,true,info);
						return;				
					}
					posx+=CUBESIZE;
				}
				posx = chunk->x - CHUNKSIZE/2 * CUBESIZE+CUBESIZE/2;
				posz+=CUBESIZE;
			}
			posz = chunk->z - CHUNKSIZE/2 * CUBESIZE + CUBESIZE/2;
			posy +=CUBESIZE;			
		}
	}
}

void World::placeCube(int x, int y, int z, bool update, bool info){
	Chunk* chunk = getCloserChunki(x,y,z);

	int posx = chunk->x-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;
	int posy = chunk->y-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;
	int posz = chunk->z-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;

	for(int yi = 0 ; yi < CHUNKSIZE ; ++yi){
		for(int zi = 0 ; zi < CHUNKSIZE ; ++zi){
			for(int xi = 0 ; xi < CHUNKSIZE ; ++xi){
				if((chunk->cubes[xi][yi][zi]==0)&&(y+CUBESIZE/2 > posy && y-CUBESIZE/2 < posy)&&(x+CUBESIZE/2 > posx && x-CUBESIZE/2 < posx)&&(z+CUBESIZE/2 > posz && z-CUBESIZE/2 < posz)){	
					chunk->placeCube(xi,yi,zi,update,info);
					return;				
				}
				posx+=CUBESIZE;
			}
			posx = chunk->x - CHUNKSIZE/2 * CUBESIZE+CUBESIZE/2;
			posz+=CUBESIZE;
		}
		posz = chunk->z - CHUNKSIZE/2 * CUBESIZE + CUBESIZE/2;
		posy +=CUBESIZE;			
	}

}

void World::freeMemory(){
	for(int i = 0 ; i < SIZE*CHUNKSIZE ; ++i)
		delete[] terrainHeightMap[i]; 
	delete[] terrainHeightMap;
}