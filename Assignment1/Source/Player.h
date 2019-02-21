#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Spring.h"

class Player 
{

private:

	Object *leftHand, *rightHand;
	Object *leftArm, *rightArm, *body;

	std::vector<Object*> parts;

	Joint *leftFingers, *righFingers;
	Spring *leftSpring, *rightSpring;

public:

	Object* getLeftHand();
	void setLeftHand(Object*);

	Object* getLeftArm();
	void setLeftArm(Object*);

	Joint* getLeftFingers();
	void setLeftFingers(Joint*);

	Spring** getLeftSpring();
	bool isGrabbingLeft();
	void leftGrab(Joint* = nullptr);
	void releaseLeft();

	Object* getRightHand();
	void setRightHand(Object*);

	Object* getRightArm();
	void setRightArm(Object*);

	Joint* getRightFingers();
	void setRightFingers(Joint*);

	Spring** getRightSpring();
	bool isGrabbingRight();
	void rightGrab(Joint* = nullptr);
	void releaseRight();

	Object* getBody();
	void setBody(Object*);

	void setParts(std::vector<Object*>);
	std::vector<Object*> getParts();

};

#endif