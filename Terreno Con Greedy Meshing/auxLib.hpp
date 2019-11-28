#include "World.h"
#include "Camera.h"
#include "WorldProperties.hpp"

#include <SFML/Graphics.hpp>
#include <GL/glut.h>

#include <string.h>
#include <math.h>	

#include <iostream>

struct Vector3i{
	int x,y,z;

	Vector3i(){}

	Vector3i(int x,int y,int z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

Vector3i getLookingTo(World* world, Camera* camera){
	Vector3i vector;

	for( int offset = 0 ; offset < 500 ; ++offset){
		int x = camera->posX+(sin(camera->rotY*3.14/180)*offset);
		int y = camera->posY+(tan(-camera->rotX*3.14/180)*offset);
		int z = camera->posZ+(cos(camera->rotY*3.14/180)*offset);

		Chunk* chunk = world->getCloserChunki(x,y,z);

		int posx = chunk->x-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;
		int posy = chunk->y-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;
		int posz = chunk->z-CHUNKSIZE/2*CUBESIZE+CUBESIZE/2;

		for(int yi = 0 ; yi < CHUNKSIZE ; ++yi){
			for(int zi = 0 ; zi < CHUNKSIZE ; ++zi){
				for(int xi = 0 ; xi < CHUNKSIZE ; ++xi){
					if(chunk->cubes[xi][yi][zi]==1)
					if(y+CUBESIZE/2 > posy && y-CUBESIZE/2 < posy)
					if(x+CUBESIZE/2 > posx && x-CUBESIZE/2 < posx)
					if(z+CUBESIZE/2 > posz && z-CUBESIZE/2 < posz){	
						return vector = Vector3i(chunk->x+posx,chunk->y+posy,chunk->z+posz);
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

	return vector;
}

void loadTexture(std::string path, int id){
	GLuint idTex;

	sf::Image data;
	data.loadFromFile(path);
	data.flipVertically();	

	glGenTextures(id,&idTex);
	glBindTexture(GL_TEXTURE_2D, idTex);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, data.getSize().x, data.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, data.getPixelsPtr() );
}

void drawAxis(){
	int size = 5000;

	glBegin(GL_LINES);
	// eje X, color rojo
	glColor3f(1,0,0);
	glVertex3f(size,0,0);
	glVertex3f(size,0,0);
	// eje Y, color verde
	glColor3f(0,1,0);
	glVertex3f(0,-size,0);
	glVertex3f(0,size,0);
	// eje Z, color azul
	glColor3f(0,0,1);
	glVertex3f(0,0,-size);
	glVertex3f(0,0,size);
	glEnd();
}
