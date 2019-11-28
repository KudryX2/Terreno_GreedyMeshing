#ifndef _CHUNK_H
#define _CHUNK_H

#include "Cube.h"

class Chunk{
	public:

	int x,y,z;
	int*** cubes;
	std::vector<Cube*> visibleCubes;

	Chunk* xPosCh; Chunk* xNegCh;
	Chunk* yPosCh; Chunk* yNegCh;
	Chunk* zPosCh; Chunk* zNegCh;

	Chunk(int x, int y, int z);
	Chunk();
	
	void fillChunk(int ** terrainHeightMap, int cx, int cz);
	void clearChunk();

	void updateChunk(bool info);
	
	void removeCube(int x,int y, int z, bool update, bool info);
	void placeCube(int x,int y,int z,bool update, bool info);
};

#endif
