#ifndef _OBJECT3D_H
#define _OBJECT3D_H

#include <GL/glut.h>

#include <string>
#include <vector>

class Object3D{

public:
	std::vector<GLfloat> vertexArray;
	std::vector<int> indexArray;
	std::vector<GLfloat> normalArray; 
	std::vector<GLfloat> texCoords;

	float	x,y,z;
	int		texID, scale;
	bool	visible;
	
	void draw(GLenum mode);
	void calculateNormals();
};

#endif
