#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

class Player 
{
private:

	Object *leftArm, *rightArm, *body;
	Joint *leftHand, *rightHand;

public:

	Object* getLeftArm();
	void setLeftArm(Object*);

	Joint * getLeftHand();
	void setLeftHand(Joint*);
	void grabLeft();
	void releaseLeft();

	Object* getRightArm();
	void setRightArm(Object*);

	Joint * getRightHand();
	void setRightHand(Joint*);
	void grabRight();
	void releaseRight();

};

#endif