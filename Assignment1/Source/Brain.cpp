#include "Brain.h"

#include <iostream>

void Brain::update() 
{
	this->leftGrab = rand() % 5;
	this->rightGrab = rand() % 5;
	if (!(rand() % 20))
	{
		this->xSwing = rand() % 200 / 100.0 - 1;
		this->ySwing = rand() % 200 / 100.0 - 1;

		std::cout << xSwing << ' ' << ySwing << std::endl;

	}
}

float Brain::getXSwing() { return this->xSwing; }
float Brain::getYSwing() { return this->ySwing; }

bool Brain::isGrabbingLeft() { return this->leftGrab; }
bool Brain::isGrabbingRight() { return this->rightGrab; }