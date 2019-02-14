#include "Joint.h"

Joint::Joint(Vector3 position, bool fixed)
{
	this->current = this->previous = position;
	this->fixed = fixed;
}

bool Joint::isFixed() { return this->fixed; }

void Joint::setFixed(bool b) 
{ 
	if (fixed != b)
		this->previous = this->current;
	this->fixed = b; 
}

Vector3 Joint::getCurrent() { return this->current; }
void Joint::setCurrent(Vector3 position) { this->current = position; }

Vector3 Joint::getPrevious() { return this->previous; }

Vector3 Joint::getMomentum() { return this->current - this->previous; }

void Joint::move(Vector3 offset)
{
	Vector3 diff = (current - previous + offset) * 0.99;
	this->previous = this->current;
	this->current += diff;
}