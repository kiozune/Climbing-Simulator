#include "Bone.h"

#include "Utility.h"

void Bone::update()
{
	Vector3 a = start->getCurrent(), b = end->getCurrent();
	Vector3 diff = a - b;
	this->rotation.x = diff.y == 0 ? 0 : atan(diff.z / diff.y);
	this->rotation.y = rad(180) - (diff.x == 0 ? 0 : atan(diff.z / diff.x));
	this->rotation.z = rad(90)  + (diff.y == 0 ? rad(90) : atan(diff.x / diff.y));

	this->center.x = a.x - diff.x / 2.0;
	this->center.y = a.y - diff.y / 2.0;
	this->center.z = a.z - diff.z / 2.0;
}

Bone::Bone(Joint* start, Joint* end)
{
	this->start = start;
	this->end = end;
	this->length = (end->getCurrent() - start->getCurrent()).Length();
	this->update();
}

Joint* Bone::getStart() { return this->start; }
Joint* Bone::getEnd() { return this->end; }
float Bone::getLength() { return this->length; }
Vector3 Bone::getRotation() { return this->rotation; }
Vector3 Bone::getCenter() { return this->center; }

void Bone::constraint()
{
	Vector3 a = start->getCurrent(), b = end->getCurrent();
	Vector3 diff = b - a;	
	float ll = diff.Length();
	float fr = ((length - ll) / ll) / 2.0;

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
			end->setCurrent(end->getCurrent() + diff * 2);
		}
	} 
	else 
	{
		start->setCurrent(start->getCurrent() - diff);
		end->setCurrent(end->getCurrent() + diff);
	}
	
	this->update();
}

void Bone::accelerate(Vector3 a, float dt)
{
	if (!start->isFixed()) start->move(a * dt);
	if (!end->isFixed()) end->move(a * dt);
	this->constraint();
}