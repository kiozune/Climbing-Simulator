#ifndef SPRING_H
#define SPRING_H

#include "Joint.h"

class Spring 
{

private:

	Joint *start, *end;
	float length, minLength, maxLength, coef;

public:

	Spring(Joint* = nullptr, Joint* = nullptr, float = 0, float = 0, float = 0);

	void constraint();

};

#endif