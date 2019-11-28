#include "Cube.h"
#include "WorldProperties.hpp"

Cube::Cube(float x, float y, float z, bool xpos, bool xneg, bool ypos, bool yneg, bool zpos, bool zneg, int sx, int sy, int sz){
	this->x = x;
	this->y = y;
	this->z = z;	

	this->sx = sx;
	this->sz = sy;
	this->sz = sz;	

	if(xpos || xneg || ypos || yneg || zpos || zneg){
		this->xpos = xpos;
		this->xneg = xneg;
		this->ypos = ypos;
		this->yneg = yneg;
		this->zpos = zpos;
		this->zneg = zneg;
	
		scale = CUBESIZE/2;

		int scalex = scale*sx;
		int scaley = scale*sy;
		int scalez = scale*sz;
		
		if(xpos){
			texCoords.push_back(0); texCoords.push_back(sy);
			texCoords.push_back(sz); texCoords.push_back(sy);
			texCoords.push_back(0); texCoords.push_back(0);
			texCoords.push_back(sz); texCoords.push_back(0);

			indexArray.push_back(1); indexArray.push_back(0); indexArray.push_back(2);
			indexArray.push_back(1); indexArray.push_back(2); indexArray.push_back(3);

			vertexArray.push_back(scalex); vertexArray.push_back(scaley); vertexArray.push_back(scalez);
			vertexArray.push_back(scalex); vertexArray.push_back(scaley); vertexArray.push_back(-scalez);
			vertexArray.push_back(scalex); vertexArray.push_back(-scaley); vertexArray.push_back(scalez);
			vertexArray.push_back(scalex); vertexArray.push_back(-scaley); vertexArray.push_back(-scalez);
		}	

		if(xneg){
			texCoords.push_back(0); texCoords.push_back(sy);
			texCoords.push_back(sz); texCoords.push_back(sy);
			texCoords.push_back(0); texCoords.push_back(0);
			texCoords.push_back(sz); texCoords.push_back(0);

			indexArray.push_back((vertexArray.size()/3)+1); indexArray.push_back((vertexArray.size()/3)+0); indexArray.push_back((vertexArray.size()/3)+2);
			indexArray.push_back((vertexArray.size()/3)+1); indexArray.push_back((vertexArray.size()/3)+2); indexArray.push_back((vertexArray.size()/3)+3);

			vertexArray.push_back(-scalex); vertexArray.push_back(scaley); vertexArray.push_back(-scalez);
			vertexArray.push_back(-scalex); vertexArray.push_back(scaley); vertexArray.push_back(scalez);
			vertexArray.push_back(-scalex); vertexArray.push_back(-scaley); vertexArray.push_back(-scalez);
			vertexArray.push_back(-scalex); vertexArray.push_back(-scaley); vertexArray.push_back(scalez);
		}
		
		if(ypos){
			texCoords.push_back(0); texCoords.push_back(sz);
			texCoords.push_back(sx); texCoords.push_back(sz);
			texCoords.push_back(0); texCoords.push_back(0);
			texCoords.push_back(sx); texCoords.push_back(0);

			indexArray.push_back((vertexArray.size()/3)+1); indexArray.push_back((vertexArray.size()/3)+0); indexArray.push_back((vertexArray.size()/3)+2);
			indexArray.push_back((vertexArray.size()/3)+1); indexArray.push_back((vertexArray.size()/3)+2); indexArray.push_back((vertexArray.size()/3)+3);

			vertexArray.push_back(-scalex); vertexArray.push_back(scaley); vertexArray.push_back(-scalez);
			vertexArray.push_back(scalex); vertexArray.push_back(scaley); vertexArray.push_back(-scalez);
			vertexArray.push_back(-scalex); vertexArray.push_back(scaley); vertexArray.push_back(scalez);
			vertexArray.push_back(scalex); vertexArray.push_back(scaley); vertexArray.push_back(scalez);
		}
		if(yneg){
			texCoords.push_back(0); texCoords.push_back(sz);
			texCoords.push_back(sx); texCoords.push_back(sz);
			texCoords.push_back(0); texCoords.push_back(0);
			texCoords.push_back(sx); texCoords.push_back(0);
			
			indexArray.push_back((vertexArray.size()/3)+2); indexArray.push_back((vertexArray.size()/3)+0); indexArray.push_back((vertexArray.size()/3)+1);
			indexArray.push_back((vertexArray.size()/3)+3); indexArray.push_back((vertexArray.size()/3)+2); indexArray.push_back((vertexArray.size()/3)+1);

			vertexArray.push_back(scalex); vertexArray.push_back(-scaley); vertexArray.push_back(scalez);
			vertexArray.push_back(-scalex); vertexArray.push_back(-scaley); vertexArray.push_back(scalez);
			vertexArray.push_back(scalex); vertexArray.push_back(-scaley); vertexArray.push_back(-scalez);
			vertexArray.push_back(-scalex); vertexArray.push_back(-scaley); vertexArray.push_back(-scalez);
		}

		if(zpos){
			texCoords.push_back(0); texCoords.push_back(sy);
			texCoords.push_back(sx); texCoords.push_back(sy);
			texCoords.push_back(0); texCoords.push_back(0);
			texCoords.push_back(sx); texCoords.push_back(0);

			indexArray.push_back((vertexArray.size()/3)+1); indexArray.push_back((vertexArray.size()/3)+0); indexArray.push_back((vertexArray.size()/3)+2);
			indexArray.push_back((vertexArray.size()/3)+1); indexArray.push_back((vertexArray.size()/3)+2); indexArray.push_back((vertexArray.size()/3)+3);

			vertexArray.push_back(-scalex); vertexArray.push_back(scaley); vertexArray.push_back(scalez);
			vertexArray.push_back(scalex); vertexArray.push_back(scaley); vertexArray.push_back(scalez);
			vertexArray.push_back(-scalex); vertexArray.push_back(-scaley); vertexArray.push_back(scalez);
			vertexArray.push_back(scalex); vertexArray.push_back(-scaley); vertexArray.push_back(scalez);
		}
		if(zneg){
			texCoords.push_back(0); texCoords.push_back(sy);
			texCoords.push_back(sx); texCoords.push_back(sy);
			texCoords.push_back(0); texCoords.push_back(0);
			texCoords.push_back(sx); texCoords.push_back(0);

			indexArray.push_back((vertexArray.size()/3)+1); indexArray.push_back((vertexArray.size()/3)+0); indexArray.push_back((vertexArray.size()/3)+2);
			indexArray.push_back((vertexArray.size()/3)+1); indexArray.push_back((vertexArray.size()/3)+2); indexArray.push_back((vertexArray.size()/3)+3);

			vertexArray.push_back(scalex); vertexArray.push_back(scaley); vertexArray.push_back(-scalez);
			vertexArray.push_back(-scalex); vertexArray.push_back(scaley); vertexArray.push_back(-scalez);
			vertexArray.push_back(scalex); vertexArray.push_back(-scaley); vertexArray.push_back(-scalez);
			vertexArray.push_back(-scalex); vertexArray.push_back(-scaley); vertexArray.push_back(-scalez);
		}

	}	
	
}