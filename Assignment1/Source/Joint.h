#ifndef JOINT_H
#define JOINT_h

#include "Vector3.h"

class Joint 
{
	
private:

	Vector3 current, previous;
	bool fixed;

public:

	Joint(Vector3 = Vector3(), bool = false);

	bool isFixed();
	void setFixed(bool);

	Vector3 getCurrent();
	void setCurrent(Vector3);
	
	Vector3 getPrevious();

	void move(Vector3 = Vector3());

};

#endif