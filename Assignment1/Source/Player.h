#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

class Player 
{

private:

	Object *leftHand, *rightHand;
	Object *leftArm, *rightArm, *body;
	Joint *leftFingers, *righFingers;

public:

	Object* getLeftHand();
	void setLeftHand(Object*);

	Object* getLeftArm();
	void setLeftArm(Object*);

	bool isLeftGrabbing();
	Joint* getLeftFingers();
	void setLeftFingers(Joint*);
	void grabLeft();
	void releaseLeft();

	Object* getRightHand();
	void setRightHand(Object*);

	Object* getRightArm();
	void setRightArm(Object*);

	bool isRightGrabbing();
	Joint* getRightFingers();
	void setRightFingers(Joint*);
	void grabRight();
	void releaseRight();

	Object* getBody();
	void setBody(Object*);

};

#endif