#ifndef _CUBE_H
#define _CUBE_H

#include "Object3D.h"

class Cube:public Object3D{
	public:	
	
	int		sx,sy,sz;	//size
	bool	xpos,xneg,
			ypos,yneg,
			zpos,zneg;	//faces
	
	Cube(float x, float y, float z,	bool xpos, bool xneg, bool ypos, bool yneg, bool zpos, bool zneg, int sx, int sy, int sz);
};

#endif
