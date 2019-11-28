#ifndef _CAMERA_H
#define _CAMERA_H

class Camera{

public:
	float	posX,posY,posZ,
			rotX,rotY;
	int		zoom;	
	bool	followingCamera;

	Camera(float posX,float posY,float posZ,float rotX,float rotY,bool followingCamera);
	void processImput(char key, int x, int y);
	void moveCamera();
};

#endif
