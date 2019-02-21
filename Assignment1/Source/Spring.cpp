#include "Spring.h"

Spring::Spring(Joint* start, Joint* end, float min, float max, float c) 
{
	this->start = start;
	this->end = end;
	this->length = (start->getCurrent() - end->getCurrent()).Length();
	this->minLength = this->length * min;
	this->maxLength = this->length * max;
	this->coef = c;
	this->enabled = true;
}

void Spring::constraint() 
{
	if (!enabled) return;
 	Vector3 a = start->getCurrent(), b = end->getCurrent();
	Vector3 diff = b - a;
	float ll = diff.Length();

	float l = ll;
	if (ll < minLength)
		l = minLength;
	else if (ll > maxLength)
		l = maxLength;

	float fr = (((length - ll) / ll) / 2.0) * coef;

	diff = diff * fr;

	if (end->isFixed())
	{
		if (!start->isFixed())
		{
			start->setCurrent(start->getCurrent() - diff * 2);
		}
	}
	else if (start->isFixed())
	{
		if (!end->isFixed())
		{
			Vector3 result = end->getCurrent() + diff * 2;
			end->setCurrent(result);
		}
	}
	else
	{
		start->setCurrent(start->getCurrent() - diff);
		end->setCurrent(end->getCurrent() + diff);
	}
}

void Spring::changeEnd(Joint* joint) { this->end = joint; }

void Spring::enable() { this->enabled = true; }
void Spring::disable() { this->enabled = false; }
bool Spring::isEnabled() { return this->enabled; }