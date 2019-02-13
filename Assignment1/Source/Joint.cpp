#include "Joint.h"

Joint::Joint(Vector3 position, bool fixed)
{
	this->current = this->previous = position;
	this->fixed = fixed;
}

bool Joint::isFixed() { return this->fixed; }

Vector3 Joint::getCurrent() { return this->current; }
void Joint::setCurrent(Vector3 position) { this->current = position; }

Vector3 Joint::getPrevious() { return this->previous; }

void Joint::move(Vector3 offset)
{
	Vector3 diff = (current - previous + offset) * 0.999;
	this->previous = this->current;
	this->current += diff;
}