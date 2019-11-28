#include <GL/gl.h>
#include <GL/glut.h>

#include "Display.h"

Display::Display(bool fullScreen){
	//Vars
	this->fullScreen = fullScreen;
	int windPosX=400, windPosY=150, windWidth=820, windHeight=600;
	GLfloat	Front_plane = 50,Back_plane = 5000;
	//Create
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(windPosX,windPosY);
	glutInitWindowSize(windWidth,windHeight);
	glutCreateWindow("SENG");
	//FullScreen
	if(fullScreen) glutFullScreen();
	//Init
	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-windWidth/10,windWidth/10,-windHeight/10,windHeight/10,Front_plane,Back_plane);	
	glViewport(0,0,windWidth,windHeight);	
	//Cursor
	glutWarpPointer(windWidth/2, windHeight/2);
	glutSetCursor(GLUT_CURSOR_NONE);
}
