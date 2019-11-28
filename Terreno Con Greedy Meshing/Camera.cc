#include <GL/gl.h>
#include <GL/glut.h>

#include "math.h"
#include <iostream>

#include "Camera.h"

Camera::Camera(float posX,float posY,float posZ,float rotX,float rotY,bool followingCamera){
	this->posX = posX;
	this->posY = posY;
	this->posZ = posZ;
	
	this->rotX = rotX;
	this->rotY = rotY;

	zoom = 400;
	
	this->followingCamera = followingCamera;
}

void Camera::processImput(char key, int x, int y){
	//Mode
	if(toupper(key)=='C') if(followingCamera) followingCamera = false;
			      else followingCamera = true;

	//Mouse imput
	int windWidth=720, windHeight=500;
	float mouseSpeed = 2; //tmp
	
	if(x > windWidth/2){
		rotY-=mouseSpeed;
		glutWarpPointer(windWidth/2, windHeight/2);
	}
	else if(x < windWidth/2){
		rotY+=mouseSpeed;
		glutWarpPointer(windWidth/2, windHeight/2);
	}
	
	if(y > windHeight/2){
		if(rotX < 85) rotX+=mouseSpeed;
		glutWarpPointer(windWidth/2, windHeight/2);
	}else if(y < windHeight/2){
		if(rotX > -85) rotX-=mouseSpeed;
		glutWarpPointer(windWidth/2, windHeight/2);
	}

	//Keyboard imput
	if(followingCamera){
		//Third person camera
		if(toupper(key)=='W') zoom-=15;		
		if(toupper(key)=='S') zoom+=15;

		int dist = zoom * cos((rotX)*3.14/180);
		posX=dist*cos((-rotY)*3.14/180);				
		posZ=dist*sin((-rotY)*3.14/180);
		posY=dist*tan((rotX)*3.14/180);		
	}else{
		//First person camera
		int speed = 20; //tmp	
		if(toupper(key)=='W'){
			posX+=speed*sin(rotY*3.14/180);				
			posZ+=speed*cos(rotY*3.14/180);
		}
		if(toupper(key)=='S'){
			posX-=speed*sin(rotY*3.14/180);				
			posZ-=speed*cos(rotY*3.14/180);
		}
		if(toupper(key)=='A'){
			posX-=speed*sin((rotY-90)*3.14/180);				
			posZ-=speed*cos((rotY-90)*3.14/180);
		}			
		if(toupper(key)=='D'){
			posX+=speed*sin((rotY-90)*3.14/180);				
			posZ+=speed*cos((rotY-90)*3.14/180);
		}

		if(toupper(key)=='O'){
			posY+=speed;
		}	
		if(toupper(key)=='L'){
			posY-=speed;
		}	
	}

//	std::cout << "Rotation X:" << rotX << " Y: " << rotY << std::endl;
//	std::cout << "Position X:" << posX << " Y: " << posY << " Z: " << posZ << std::endl;
	moveCamera();

	glutPostRedisplay();
}

void Camera::moveCamera(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(followingCamera){
		gluLookAt(posX, posY, posZ , 0.0f , 0.0f , 0.0f ,0.0f, 1.0f,  0.0f);
	}else{
		gluLookAt(posX, posY, posZ , posX+sin(rotY*3.14/180) ,
					     posY+tan(-rotX*3.14/180) ,
					     posZ+cos(rotY*3.14/180),0.0f, 1.0f,  0.0f);
	}
}
