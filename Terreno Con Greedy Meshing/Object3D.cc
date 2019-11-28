#include <GL/gl.h>
#include <GL/glut.h>

#include "Object3D.h"

void Object3D::draw(GLenum mode){
	glPushMatrix();
	glTranslatef(x,y,z);
	
	if(normalArray.size() > 0){
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, normalArray.size(), &normalArray[0]);
	}
	
	if(texCoords.size() > 0){
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertexArray[0]);

	glDrawElements(GL_TRIANGLES,indexArray.size(),GL_UNSIGNED_INT,&indexArray[0]);
	
	glPopMatrix();
}

void Object3D::calculateNormals(){

	std::vector<GLfloat> triangle,u,v;

	for(int i = 0 ; i < indexArray.size()-3 ; i+=3){
		triangle.clear(); u.clear(); v.clear();
  	
		// tr1 (p)
		triangle.push_back(vertexArray[indexArray[i]]);
		triangle.push_back(vertexArray[indexArray[i+1]]);
		triangle.push_back(vertexArray[indexArray[i+2]]);
		// tr2 (q)
		triangle.push_back(vertexArray[indexArray[i+3]]);
		triangle.push_back(vertexArray[indexArray[i+4]]);
		triangle.push_back(vertexArray[indexArray[i+5]]);
		// tr3 (r)
		triangle.push_back(vertexArray[indexArray[i+6]]);
		triangle.push_back(vertexArray[indexArray[i+7]]);
		triangle.push_back(vertexArray[indexArray[i+8]]);
		  
		// a
		u.push_back(triangle[3] - triangle[0]); // tr2 - tr1 (q - p)
		u.push_back(triangle[4] - triangle[1]);
		u.push_back(triangle[5] - triangle[2]);
		// b
		v.push_back(triangle[6] - triangle[0]); // tr3 - tr1 (r - p)
		v.push_back(triangle[7] - triangle[1]);
		v.push_back(triangle[8] - triangle[2]);

		normalArray.push_back(u[1]*v[2] - u[2]*v[1]);  // u.y * v.z - u.z * v.y
		normalArray.push_back(u[2]*v[0] - u[0]*v[2]);  // u.z * v.x - u.x * v.z
		normalArray.push_back(u[0]*v[1] - u[1]*v[0]);  // u.x * v.y - u.y * v.x 
	}

}

