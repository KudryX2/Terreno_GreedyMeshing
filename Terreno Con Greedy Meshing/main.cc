#include <GL/glew.h>
#include <GL/glut.h>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "math.h"
#include "time.h"

#include "Display.h"
#include "Camera.h"

#include "Object3D.h"	
#include "World.h"
 
#include "auxLib.hpp"
#include "WorldProperties.hpp"

//Display
Display* display;
int t,tb,frame= 0;
bool fps = false;
int lastDrawTime = 0;
bool drawTime = false;

//Camera
Camera* camera;
bool displayPos = false;

//WORLD
World* world;
bool info = false;

//Render
GLenum mode = GL_TRIANGLES;
bool culling = true;
bool axis = false;

//Keyboard
bool* keys = new bool[16];


using namespace std;

void loadTextures(){	
	loadTexture("tex/wood.jpg", 0);
//	loadTexture("tex/grass.jpg", 1);		
//	loadTexture("tex/Test1.bmp", 4);
}

void prepareToDraw(){
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK,mode);		
	//Texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);	
}

void draw(void){	
	//Clear		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Draw axis
	if(axis) drawAxis(); 
	//Draw World and update interactive chunks
	world->interactiveChunks.clear();

	int dist;
	glEnable(GL_TEXTURE_2D);
	for(int y = 0 ; y < HEIGHT ; ++y){
		for(int x = 0 ; x < SIZE ; ++x){
			for(int z = 0 ; z < SIZE ; ++z){
				dist = pow((pow(world->chunks[x][y][z]->x-camera->posX,2)+pow(world->chunks[x][y][z]->y-camera->posY,2)+pow(world->chunks[x][y][z]->z-camera->posZ,2)),(float)1/3); 
				if(dist < 300)
				//	if(z%2 == 0) glColor3f(0,1,0); else glColor3f(1,0,1);		
					for(int i = 0 ; i < (world->chunks[x][y][z]->visibleCubes.size()) ; ++i)
						world->chunks[x][y][z]->visibleCubes[i]->draw(mode); 
					
					
				if(dist < 150) world->interactiveChunks.push_back(world->chunks[x][y][z]);
			}
		}
	}	

	glDisable(GL_TEXTURE_2D);

/* * /
	Vector3i lookingTo = getLookingTo(world,camera);
	glTranslatef(lookingTo.x, lookingTo.y, lookingTo.z);
	glutSolidSphere(5,10,10);
	glTranslatef(-lookingTo.x, -lookingTo.y, -lookingTo.z);
/* */

	// FPS
	if(fps){
		frame++;
		t=glutGet(GLUT_ELAPSED_TIME);
		if (t - tb > 1000) {
			cout << frame*1000.0/(t-tb) << endl;
		 	tb = t;
			frame = 0;
		}
	}
	
	if(drawTime){
		t=glutGet(GLUT_ELAPSED_TIME);
		cout << t-lastDrawTime << " mil" << endl;
		lastDrawTime = t;
	}

	glutSwapBuffers();
	glutPostRedisplay();	
}

void mousePassiveListener(int x, int y){	
	camera->processImput('n',x,y);
}

void keyListener(unsigned char key,int x,int y){

	camera->processImput(key,x,y);

	if(toupper(key)=='Q') exit(0);
	
	if(toupper(key)=='1'){ mode = GL_POINT;	glPolygonMode(GL_FRONT_AND_BACK,mode); glColor3f(0,1,0); glDisable(GL_TEXTURE_2D);}		
	if(toupper(key)=='2'){ mode = GL_LINE;  glPolygonMode(GL_FRONT_AND_BACK,mode); glColor3f(0,1,0); glDisable(GL_TEXTURE_2D);}		
	if(toupper(key)=='3'){
		mode = GL_FILL;
		glPolygonMode(GL_FRONT_AND_BACK,mode);
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
 	}

	if(toupper(key)=='4') 	if(culling){
					culling = false;
					glDisable(GL_CULL_FACE); 
				}else{
					culling = true;
					glEnable(GL_CULL_FACE);
				}

	if(toupper(key)=='E') world->destroyCube(camera, info);
	if(toupper(key)=='R') world->placeCube(camera, info);
	

	if(toupper(key)=='T'){
		string cmd;		
		cout << "\nIntroduce el comando" << endl;
		cout << "fps | drawt | axis | pos | spos | info" << endl;
		cin >> cmd;
	
		if(cmd == "fps")   if(fps) fps = false; else fps = true;
		if(cmd == "drawt") if(drawTime) drawTime = false; else drawTime = true;
		if(cmd == "axis")  if(axis) axis = false; else axis = true;
		if(cmd == "pos")   if(displayPos) displayPos = false; else displayPos = true;	
		if(cmd == "spos"){
			int x,y,z;
			cout << "\nIntroduce la posicion";
			cin >> x; cin >> y; cin >> z;
			camera->posX = x;
			camera->posY = y;
			camera->posZ = z;
		}
		if(cmd == "info")  if(info) info = false; else info = true;

	}

	if(displayPos)
		cout << "Position X:" << camera->posX << " Y: " << camera->posY << " Z: " << camera->posZ << endl;
 
}

int main(int argc, char **argv){
	cout << "OpenGL Init" << endl;
	glutInit(&argc, argv);

/*
	if (glewIsSupported("GL_VERSION_2_0"))
		cout << "Ready for OpenGL 2.0\n";
	else {
		cout << "OpenGL 2.0 not supported\n";
	}
*/

	cout << "Camera Init" << endl;
	camera = new Camera(0,1000,0,0,0,false);	

	cout << "World loading";
	world = new World();

	cout << "OpenGL Create and init display" << endl;
	display = new Display(false);

	cout << "OpenGL Events" << endl;
	prepareToDraw();
	glutDisplayFunc(draw);          
	glutKeyboardFunc(keyListener);
	glutPassiveMotionFunc(mousePassiveListener);

	cout << "Textures loading" << endl;
	loadTextures();

	cout << "OpenGL mainLoop" << endl;
	glutMainLoop();

	cout << "EXIT" << endl;
}