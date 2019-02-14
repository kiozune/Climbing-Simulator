#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

class Player 
{
private:

	Object *leftArm, *rightArm, *body;
	Joint *leftFingers, *righFingers;

public:

	Object* getLeftArm();
	void setLeftArm(Object*);

	bool isLeftGrabbing();
	void setLeftFingers(Joint*);
	void grabLeft();
	void releaseLeft();

	Object* getRightArm();
	void setRightArm(Object*);

	bool isRightGrabbing();
	void setRightFingers(Joint*);
	void grabRight();
	void releaseRight();

	Object* getBody();
	void setBody(Object*);

};

#endif