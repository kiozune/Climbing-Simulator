#ifndef CAMERA_H
#define CAMERA_H

#include "GL/glew.h"
#include "Vector3.h"

#include "Application.h"

class Camera {

protected:

	Vector3 position;

	Vector3 worldUp;
	Vector3 front;
	Vector3 right;
	Vector3 up;

	Vector3 target;

	virtual void update() = 0;

public:

	void Init(const Vector3&);

	Vector3 getPosition();
	void setPosition(Vector3 pos);

	Vector3 getTarget();
	virtual void setTarget(Vector3 target) = 0;

	Vector3 getUp();

};

#endif