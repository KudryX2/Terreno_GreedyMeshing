#include <algorithm>
#include <math.h>

#include <iostream>

#include "Chunk.h"
#include "WorldProperties.hpp"

using namespace std;

Chunk::Chunk(int x, int y, int z){
	this->x = x;
	this->y = y;
	this->z = z;

	cubes = new int**[CHUNKSIZE];	
	for(int i = 0 ; i < CHUNKSIZE ; ++i){
		cubes[i] = new int*[CHUNKSIZE];
		for(int j = 0 ; j < CHUNKSIZE ; ++j){
			cubes[i][j] = new int[CHUNKSIZE];
		}
	}
}

Chunk::Chunk(){}

void Chunk::fillChunk(int ** terrainHeightMap, int cx, int cz){
	int chunkHeight = y - CHUNKSIZE*CUBESIZE/2;

	for(int yi = 0 ; yi < CHUNKSIZE ; ++yi)
		for(int zi = 0 ; zi < CHUNKSIZE ; ++zi)
			for(int xi = 0 ; xi < CHUNKSIZE ; ++xi){
				if(chunkHeight + yi*CUBESIZE < terrainHeightMap[cx*CHUNKSIZE +xi][cz*CHUNKSIZE +zi])
					cubes[xi][yi][zi] = 1;	
				else 
					cubes[xi][yi][zi] = 0;

			}

}

void Chunk::clearChunk(){
	for(int i = 0 ; i < CHUNKSIZE ; ++i){
		for(int j = 0 ; j < CHUNKSIZE ; ++j){
			delete[] cubes[i][j];
		}	
		delete[] cubes[i];
	}
	delete[] cubes;
}

void Chunk::updateChunk(bool info){
	// CLEAR PREVIOUS DATA
	for(int i = 0 ; i < visibleCubes.size() ; ++i) delete visibleCubes[i]; 
	visibleCubes.clear();

	// AUXILIAR VARIABLES
	Cube**** xcubes = new Cube***[CHUNKSIZE];
	Cube**** xzcubes = new Cube***[CHUNKSIZE];	
	
	for(int i = 0 ; i < CHUNKSIZE ; ++i){
		xcubes[i] = new Cube**[CHUNKSIZE];
		xzcubes[i] = new Cube**[CHUNKSIZE];
		for(int j = 0 ; j < CHUNKSIZE ; ++j){
			xcubes[i][j] = new Cube*[CHUNKSIZE];
			xzcubes[i][j] = new Cube*[CHUNKSIZE];
		}
	}

	for(int yc = 0 ; yc < CHUNKSIZE ; ++yc)
		for(int zc = 0 ; zc < CHUNKSIZE ; ++zc)
			for(int xc = 0 ; xc < CHUNKSIZE ; ++xc){
				xcubes[xc][yc][zc] = NULL;	
				xzcubes[xc][yc][zc] = NULL;	
			}

	int posx = this->x-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;
	int posy = this->y-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;
	int posz = this->z-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;

	int 	sx;
	bool 	currentZpos,currentZneg,nextZpos,nextZneg,
			currentXpos,currentXneg,nextXpos,nextXneg,
			currentYpos,currentYneg,nextYpos,nextYneg;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// FUSION ////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	// X AXIS
	for(int y = 0 ; y < CHUNKSIZE ; ++y){
		for(int z = 0 ; z < CHUNKSIZE ; ++z){
			sx = 1;
			for(int x = 0 ; x < CHUNKSIZE ; ++x){
				if(cubes[x][y][z]==1){		
					// Y
					if(y < CHUNKSIZE-1 && cubes[x][y+1][z]==1) 	currentYpos = false; else currentYpos = true;
					if(y > 0 && cubes[x][y-1][z] == 1)			currentYneg = false; else currentYneg = true;

					if(x < CHUNKSIZE-1 && y < CHUNKSIZE-1 && cubes[x+1][y+1][z]==1)	nextYpos = false; else nextYpos = true;
					if(x < CHUNKSIZE-1 && y > 0 && cubes[x+1][y-1][z] == 1) 		nextYneg = false; else nextYneg = true;

					// Y INTERCHUNK
					if(yPosCh != NULL && y == CHUNKSIZE-1 && yPosCh->cubes[x][0][z] == 1) currentYpos = false;
					if(yNegCh != NULL && y == 0 && yNegCh->cubes[x][CHUNKSIZE-1][z] == 1) currentYneg = false;

					if(yPosCh != NULL && y == CHUNKSIZE-1 && x < CHUNKSIZE-1 && yPosCh->cubes[x+1][0][z] == 1) nextYpos = false;
					if(yNegCh != NULL && y == 0 && x < CHUNKSIZE-1 && yNegCh->cubes[x+1][CHUNKSIZE-1][z] == 1) nextYneg = false;

					// Z
					if(z < CHUNKSIZE-1 && cubes[x][y][z+1]==1)	currentZpos = false; else currentZpos = true;
					if(z > 0 && cubes[x][y][z-1]==1)           	currentZneg = false; else currentZneg = true;
	
					if((x<CHUNKSIZE-1)&&(z<CHUNKSIZE-1)&(cubes[x+1][y][z+1]==1)) nextZpos = false; else nextZpos = true;
					if((x<CHUNKSIZE-1)&&(z>0)&(cubes[x+1][y][z-1]==1))           nextZneg = false; else nextZneg = true;				

					// Z INTERCHUNK
					if(zPosCh != NULL && z == CHUNKSIZE-1 && zPosCh->cubes[x][y][0] == 1) currentZpos = false;
					if(zNegCh != NULL && z == 0 && zNegCh->cubes[x][y][CHUNKSIZE-1] == 1) currentZneg = false;

					if(zPosCh != NULL && z == CHUNKSIZE-1 && x < CHUNKSIZE-1 && zPosCh->cubes[x+1][y][0] == 1) nextZpos = false;
					if(zNegCh != NULL && z == 0 && x < CHUNKSIZE-1 && zNegCh->cubes[x+1][y][CHUNKSIZE-1] == 1) nextZneg = false;

					if((x<CHUNKSIZE-1)&&(cubes[x+1][y][z]==1)&&(currentZpos==nextZpos)&&(currentZneg==nextZneg)&&
						(currentYpos == nextYpos)&&(currentYneg == nextYneg)) sx++;
					else{
						xcubes[x][y][z] = new Cube(posx-(sx-1)*CUBESIZE/2,posy,posz,false,false,false,false,false,false,sx,1,1);
						
					//	visibleCubes.push_back(new Cube(posx-(sx-1)*100/2,posy,posz,currentXpos,currentXneg,currentYpos,currentYneg,currentZpos,currentZneg,sx,1,1));

						sx = 1;
					}		
				}
				posx+=CUBESIZE;
			}
			posx = this->x - CHUNKSIZE/2 * CUBESIZE + CUBESIZE/2;
			posz+=CUBESIZE;
		}
		posz = this->z - CHUNKSIZE/2 * CUBESIZE + CUBESIZE/2;
		posy +=CUBESIZE;
	}
/* */
	// XZ AXIS
	int sz;	
	for(int y = 0 ; y < CHUNKSIZE ; ++y){
		for(int x = 0 ; x < CHUNKSIZE ; ++x){
			sz = 1;
			for(int z = 0 ; z < CHUNKSIZE ; ++z){
				if(xcubes[x][y][z]!=NULL){
					// Y
					if( y == CHUNKSIZE-1 || y < CHUNKSIZE-1 && cubes[x][y+1][z]==0) currentYpos = true; else currentYpos = false;
					if( y == 0 || y > 0 && cubes[x][y-1][z]==0) 				  currentYneg = true; else currentYneg = false;
						
					if( y == CHUNKSIZE-1 || z < CHUNKSIZE-1 && y < CHUNKSIZE-1 && cubes[x][y+1][z+1] == 0) nextYpos = true; else nextYpos = false;
					if( y == 0 || z < CHUNKSIZE-1 && y > 0 && cubes[x][y-1][z+1] == 0) 					 nextYneg = true; else nextYneg = false;

					// Y INTERCHUNK
					if(yPosCh != NULL && y == CHUNKSIZE-1 && yPosCh->cubes[0][y][z] == 1) currentXpos = false;
					if(yNegCh != NULL && y == 0 && yNegCh->cubes[CHUNKSIZE-1][y][z] == 1) currentXneg = false;

					if(yPosCh != NULL && y == CHUNKSIZE-1 && z < CHUNKSIZE-1 && yPosCh->cubes[x][0][z+1] == 1) nextXpos = false;
					if(yNegCh != NULL && y == 0 && z < CHUNKSIZE-1 && yNegCh->cubes[x][CHUNKSIZE-1][z+1] == 1) nextXneg = false;

					// X
					if((x == CHUNKSIZE-1)||(x < CHUNKSIZE-1 && cubes[x+1][y][z]==0))	currentXpos = true; else currentXpos = false;
					if((x-xcubes[x][y][z]->sx+1 == 0)||(x-xcubes[x][y][z]->sx >= 0 && cubes[x-xcubes[x][y][z]->sx][y][z]==0)) currentXneg = true; else currentXneg = false;

					if((x+1 == CHUNKSIZE)||(z < CHUNKSIZE-1 && x < CHUNKSIZE-1 && cubes[x+1][y][z+1]==0)) nextXpos = true; else nextXpos = false;
					if((x-xcubes[x][y][z]->sx+1 == 0)||( z < CHUNKSIZE-1 && x-xcubes[x][y][z]->sx >= 0 && cubes[x-xcubes[x][y][z]->sx][y][z+1]==0)) nextXneg = true; else nextXneg = false;

					// x INTERCHUNK
					if(xPosCh != NULL && x == CHUNKSIZE-1 && xPosCh->cubes[0][y][z] == 1) currentXpos = false;
					if(xNegCh != NULL && x-xcubes[x][y][z]->sx+1 == 0 && xNegCh->cubes[CHUNKSIZE-1][y][z] == 1) currentXneg = false;

					if(xPosCh != NULL && x == CHUNKSIZE-1 && z < CHUNKSIZE-1 && xPosCh->cubes[0][y][z+1] == 1) nextXpos = false;
					if(xNegCh != NULL && x-xcubes[x][y][z]->sx+1 == 0 && z < CHUNKSIZE-1 && xNegCh->cubes[CHUNKSIZE-1][y][z+1] == 1) nextXneg = false;

					if((z < CHUNKSIZE-1)&&(xcubes[x][y][z+1]!=NULL)&&(xcubes[x][y][z+1]->sx == xcubes[x][y][z]->sx )&&
						(currentXpos == nextXpos && currentXneg == nextXneg )&&
						(currentYpos == nextYpos && currentYneg == nextYneg )) sz++;
					else{
					/* */	
						xzcubes[x][y][z]=new Cube(xcubes[x][y][z]->x,xcubes[x][y][z]->y,xcubes[x][y][z]->z-(sz-1)*CUBESIZE/2,
							false,false,false,false,false,false,xcubes[x][y][z]->sx,1,sz);				
					/* * / 
						visibleCubes.push_back(new Cube(xcubes[x][y][z]->x,xcubes[x][y][z]->y,xcubes[x][y][z]->z-(sz-1)*100/2,
							currentXpos,currentXneg,currentYpos,currentYneg,currentZpos,currentZneg,
							xcubes[x][y][z]->sx,1,sz));
					/* */ 
						sz = 1;	
					} 
				}		
			}
		}
	}
/* */	
	// XZY AXIS
	int sy;	
	for(int x = 0 ; x < CHUNKSIZE ; ++x){
		for(int z = 0 ; z < CHUNKSIZE ; ++z){
			sy = 1;
			for(int y = 0 ; y < CHUNKSIZE ; ++y){
				
				if(xzcubes[x][y][z]!=NULL){
					// x				
					if(x == CHUNKSIZE-1 || x < CHUNKSIZE-1 && cubes[x+1][y][z] == 0) currentXpos = true; else currentXpos = false;
					if(x == 0 ||(x-xzcubes[x][y][z]->sx+1 == 0)||(x-xzcubes[x][y][z]->sx >= 0 && cubes[x-xzcubes[x][y][z]->sx][y][z]==0)) currentXneg = true; else currentXneg = false;
	
					if(x == CHUNKSIZE-1 || y < CHUNKSIZE-1 && x < CHUNKSIZE-1 && cubes[x+1][y+1][z] == 0)	nextXpos = true; else nextXpos = false;
					if(x-xzcubes[x][y][z]->sx+1 == 0 || y < CHUNKSIZE-1 && x-xzcubes[x][y][z]->sx >= 0 && cubes[x-xzcubes[x][y][z]->sx][y+1][z]==0) nextXneg = true; else nextXneg = false;
				
					// X INTERCHUNK
					if(xPosCh != NULL && x == CHUNKSIZE-1  && xPosCh->cubes[0][y][z] == 1) currentXpos = false;
					if(xNegCh != NULL && x-xzcubes[x][y][z]->sx+1 == 0 && xNegCh->cubes[CHUNKSIZE-1][y][z] == 1) currentXneg = false;

					if(xPosCh != NULL && y < CHUNKSIZE-1 && x == CHUNKSIZE-1  && xPosCh->cubes[0][y+1][z] == 1) nextXpos = false;
					if(xNegCh != NULL && y < CHUNKSIZE-1 && x-xzcubes[x][y][z]->sx+1 == 0 && xNegCh->cubes[CHUNKSIZE-1][y+1][z] == 1) nextXneg = false;


					// Z
					if(z == CHUNKSIZE-1 || z < CHUNKSIZE-1 && cubes[x][y][z+1] == 0) currentZpos = true; else currentZpos = false;
					if(z == 0 ||(z-xzcubes[x][y][z]->sz+1 == 0)||(z-xzcubes[x][y][z]->sz >= 0 && cubes[x][y][z-xzcubes[x][y][z]->sz]==0)) currentZneg = true; else currentZneg = false;

					if(z == CHUNKSIZE-1 || y < CHUNKSIZE-1 && z < CHUNKSIZE-1 && cubes[x][y+1][z+1] == 0) 	nextZpos = true; else nextZpos = false;
					if(z-xzcubes[x][y][z]->sz+1 == 0 || y < CHUNKSIZE-1 && z-xzcubes[x][y][z]->sz >= 0 && cubes[x][y+1][z-xzcubes[x][y][z]->sz]==0) nextZneg = true; else nextZneg = false;
				
					// Z INTERCHUNK
					if(zPosCh != NULL && z == CHUNKSIZE-1 && zPosCh->cubes[x][y][0] == 1) currentZpos = false;
					if(zNegCh != NULL && z-xzcubes[x][y][z]->sz+1 == 0 && zNegCh->cubes[x][y][CHUNKSIZE-1] == 1) currentZneg = false;

					if(zPosCh != NULL && y < CHUNKSIZE-1 && z == CHUNKSIZE-1 && zPosCh->cubes[x][y+1][0] == 1) nextZpos = false;
					if(zNegCh != NULL && y < CHUNKSIZE-1 && z-xzcubes[x][y][z]->sz+1 == 0 && zNegCh->cubes[x][y+1][CHUNKSIZE-1] == 1) nextZneg = false;
	
					if((y < CHUNKSIZE-1)&&(xzcubes[x][y+1][z]!=NULL)&&
						(xzcubes[x][y+1][z]->sx == xzcubes[x][y][z]->sx)&&
						(xzcubes[x][y+1][z]->sz == xzcubes[x][y][z]->sz )&&
						(currentXpos == nextXpos)&&
						(currentXneg == nextXneg)&&
						(currentZpos == nextZpos)&&
						(currentZneg == nextZneg)) sy++;
					else{
						// Y
						if( y == CHUNKSIZE-1 || y < CHUNKSIZE-1 && cubes[x][y+1][z]==0) currentYpos = true; else currentYpos = false;
						if( y-sy+1 == 0 || y-sy+1 > 0 && cubes[x][y-sy][z]==0) 			currentYneg = true; else currentYneg = false;
				
						// Y INTERCHUNK
						if(yPosCh != NULL && y == CHUNKSIZE-1 && yPosCh->cubes[x][0][z] == 1) currentYpos = false;
						if(yNegCh != NULL && y-sy+1 == 0 && yNegCh->cubes[x][CHUNKSIZE-1][z] == 1) currentYneg = false;

						visibleCubes.push_back(new Cube(xzcubes[x][y][z]->x,xzcubes[x][y][z]->y-(sy-1)*CUBESIZE/2,xzcubes[x][y][z]->z,
										currentXpos,currentXneg,currentYpos,currentYneg,currentZpos,currentZneg,
										xzcubes[x][y][z]->sx,sy,xzcubes[x][y][z]->sz));

						sy = 1;
					}
					 
				}		
				
			}
		}
	}	

	// CLEAN
	for(int i = 0 ; i < CHUNKSIZE ; ++i){
		for(int j = 0 ; j < CHUNKSIZE ; ++j){
			delete[] xcubes[i][j];
			delete[] xzcubes[i][j];
		}	
		delete[] xcubes[i];
		delete[] xzcubes[i];
	}
	delete[] xcubes;
	delete[] xzcubes;

	// INFO
	if(info) cout << "Chunk updated " << visibleCubes.size() << endl;
}


void Chunk::removeCube(int x,int y,int z,bool update,bool info){
	cubes[x][y][z] = 0;	
	if(update){
		updateChunk(info);
		if(y == 0 && yNegCh != NULL) 			yNegCh->updateChunk(info);
		if(y == CHUNKSIZE-1 && yPosCh != NULL)  yPosCh->updateChunk(info);
		if(x == 0 && xNegCh != NULL) 			xNegCh->updateChunk(info);
		if(x == CHUNKSIZE-1 && xPosCh != NULL)  xPosCh->updateChunk(info);
		if(z == 0 && zNegCh != NULL)	 		zNegCh->updateChunk(info); 
		if(z == CHUNKSIZE-1 && zPosCh != NULL)  zPosCh->updateChunk(info);
	}
}

void Chunk::placeCube(int x,int y,int z,bool update,bool info){
	cubes[x][y][z] = 1;	
	if(update){
	updateChunk(info);
		if(y == 0 && yNegCh != NULL) 			yNegCh->updateChunk(info);
		if(y == CHUNKSIZE-1 && yPosCh != NULL)  yPosCh->updateChunk(info);
		if(x == 0 && xNegCh != NULL) 			xNegCh->updateChunk(info);
		if(x == CHUNKSIZE-1 && xPosCh != NULL)  xPosCh->updateChunk(info);
		if(z == 0 && zNegCh != NULL) 			zNegCh->updateChunk(info);
		if(z == CHUNKSIZE-1 && zPosCh != NULL)  zPosCh->updateChunk(info);
	}
}
/*
//vars
	int posx = this->x-size/2*100+50;
	int posy = this->y-size/2*100+50;
	int posz = this->z-size/2*100+50;

	bool back = true; bool front = true;
	bool left = true; bool right = true;
	bool up = true;	  bool down = true;

	for(int y = 0 ; y < size ; ++y){
		for(int z = 0 ; z < size ; ++z){
			for(int x = 0 ; x < size ; ++x){

				back = true; front = true;
				left = true; right = true;
				up = true; 	 down = true;

				if(cubes[x][y][z] == 1){

					if(x > 0) 		if(cubes[x-1][y][z] == 1) left = false;
					if(x < size-1)	if(cubes[x+1][y][z] == 1) right = false;
					
					if(z > 0) 		if(cubes[x][y][z-1] == 1) front = false;
					if(z < size-1)	if(cubes[x][y][z+1] == 1) back = false;

					if(y > 0) 		if(cubes[x][y-1][z] == 1) down = false;
					if(y < size-1)	if(cubes[x][y+1][z] == 1) up = false;

					if(back || front || left || right || up || down){
						visibleCubes.push_back(new Cube(posx,posy,posz,x,y,z,back,front,left,right,up,down,1,1,1));		
					}
				}
				posx+=100;
			}	
			posx = this->x - size/2 * 100+50;
			posz+=100;
		}
		posz = this->z - size/2 * 100 + 50;
		posy +=100; 	
	}
*/
