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

void Joint::resetResolve() { this->isResolved = false; }

void Joint::displace(Vector3 d)
{
	if (this->isResolved) return;
	this->isResolved = true;
	this->current += d;
	this->previous = this->current;
}

Vector3 Joint::getCurrent() { return this->current; }
void Joint::setCurrent(Vector3 position) { this->current = position; }

Vector3 Joint::getPrevious() { return this->previous; }

void Joint::setPosition(Vector3 v) 
{ 
	this->current = v;
	this->previous = v;
}

Vector3 Joint::getMomentum() { return this->current - this->previous; }

void Joint::move(Vector3 offset)
{
	Vector3 diff = (current - previous + offset) * 0.99;
	this->previous = this->current;
	this->current += diff;
}