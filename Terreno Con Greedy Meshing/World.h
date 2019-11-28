#ifndef _WORLD_H
#define _WORLD_H

#include <iostream>

#include "math.h"

#include "Camera.h"
#include "Chunk.h"
#include "Object3D.h"

#include "WorldProperties.hpp"

class World{
	public:	

	Chunk**** chunks;
	std::vector<Chunk*> interactiveChunks;
	int ** terrainHeightMap;
	
	World();

	void createChunks();

	void generateTerrain();
	void elevateTerrain(int x, int z, int size, int hight);

	void generateStructures();
	
	Chunk* getCloserChunk(int x, int y, int z);
	Chunk* getCloserChunki(int x, int y, int z);

	void destroyCube(Camera* camera, bool info);
	void placeCube(Camera* camera, bool info);
	void placeCube(int x, int y, int z, bool update, bool info);

	void freeMemory();
};

#endif
