#include "Object.h"

#include "Utility.h"

void Object::update()
{
	Vector3 a = start->getCurrent(), b = end->getCurrent();
	Vector3 diff = a - b;
	// this->rotation.x = diff.y == 0 ? 0 : atan(diff.z / diff.y);
	this->rotation.y = rad(180) - (diff.x == 0 ? 0 : atan(diff.z / diff.x));
	this->rotation.z = rad(90) + (diff.y == 0 ? rad(90) : atan(diff.x / diff.y));

	this->center.x = a.x - diff.x / 2.0;
	this->center.y = a.y - diff.y / 2.0;
	this->center.z = a.z - diff.z / 2.0;

	bb.setRotation(rotation.y, rotation.z);
	bb.setTranslation(center.x, center.y, center.z);
}

Object::Object(Joint* start, Joint* end, float mass)
{
	this->start = start;
	this->end = end;

	this->length = (end->getCurrent() - start->getCurrent()).Length();
	
	this->bb.setVertces(Vector3(0.5, 0.5, 0.5), Vector3(-0.5, -0.5, -0.5));
	this->bb.setScale(this->length, 2, 2);
	
	this->mass = mass;
	this->update();
}

Joint* Object::getStart() { return this->start; }
Joint* Object::getEnd() { return this->end; }
float Object::getLength() { return this->length; }
Vector3 Object::getRotation() { return this->rotation; }
Vector3 Object::getCenter() { return this->center; }
BoundingBox Object::getBoundingBox() { return this->bb; }

void Object::constraint()
{
	Vector3 a = this->start->getCurrent();
	Vector3 b = this->end->getCurrent();

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

void Object::accelerate(Vector3 a, float dt)
{
	if (!start->isFixed()) start->move(a * dt);
	if (!end->isFixed()) end->move(a * dt);
	this->constraint();
}

void Object::applyImpulse(Vector3 force, float dt)
{
	if (mass == 0) return;
	Vector3 a = Vector3(force.x / mass, force.y / mass, force.z / mass);
	this->accelerate(a, dt);
}