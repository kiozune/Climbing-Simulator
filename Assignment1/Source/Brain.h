#ifndef BRAIN_H
#define BRAIN_H

class Brain
{
	
private:

	float xSwing, ySwing;
	bool leftGrab, rightGrab;

public:

	void update();

	float getXSwing();
	float getYSwing();

	bool isGrabbingLeft();
	bool isGrabbingRight();

};

#endif