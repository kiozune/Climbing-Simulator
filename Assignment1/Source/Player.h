#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "Spring.h"

enum PLAYER_STATE
{
	WON,
	ALIVE,
	DEAD
};

class Player 
{

protected:

	unsigned id;
	float energy;
	PLAYER_STATE state;

	Object *leftHand, *rightHand;
	Object *leftArm, *rightArm, *body;

	std::vector<Object*> parts;

	Joint *leftFingers, *righFingers;
	Spring *leftSpring, *rightSpring;

public:

	Player();

	unsigned getId();
	void setId(unsigned);

	PLAYER_STATE getState();
	void setState(PLAYER_STATE);

	void recover(float);
	void tire(float);
	float getEnergy();

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

	std::vector<Object*> getParts();
	void setParts(std::vector<Object*>);

};

#endif