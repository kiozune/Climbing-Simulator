#ifndef SPRING_H
#define SPRING_H

#include "Joint.h"

class Spring 
{

private:

	Joint *start, *end;
	float length, minLength, maxLength, coef;

	bool enabled;

public:

	Spring(Joint* = nullptr, Joint* = nullptr, float = 0, float = 0, float = 0);

	void constraint();

	void changeEnd(Joint*);

	void enable();
	void disable();
	bool isEnabled();
};

#endif