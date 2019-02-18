#include "Object.h"

#include "Utility.h"

void Object::update()
{
	Vector3 a = start->getCurrent(), b = end->getCurrent();
	Vector3 diff = a - b;
	float hyp = sqrt(pow(diff.z, 2) + pow(diff.x, 2));
	if (diff.x != 0) hyp *= diff.x / fabs(diff.x);
	this->rotation.y = diff.x == 0 ? 0 : atan(-diff.z / diff.x);
	this->rotation.z = atan(diff.y / hyp);

	this->center.x = a.x - diff.x / 2.0;
	this->center.y = a.y - diff.y / 2.0;
	this->center.z = a.z - diff.z / 2.0;
	
	this->bb.setRotation(this->rotation.y, this->rotation.z);
	this->bb.setTranslation(this->center);
}

Object::Object(Joint* start, Joint* end, float mass, float size)
{
	this->start = start;
	this->end = end;

	this->mass = mass;
	this->length = (end->getCurrent() - start->getCurrent()).Length();
	this->scale = Vector3(this->length, size, size);
	
	this->bb.setVertices(Vector3(0.5, 0.5, 0.5), Vector3(-0.5, -0.5, -0.5));
	this->bb.setScale(this->scale);

	this->update();

	this->affectByGravity = true;
	this->clippingEnabled = false;
}

Object::Object(Vector3 scale, Vector3 center, float mass, bool g)
{
	this->start = nullptr;
	this->end = nullptr;

	this->mass = mass;
	this->length = 0;
	this->scale = scale;
	this->center = center;

	this->bb.setVertices(Vector3(0.5, 0.5, 0.5), Vector3(-0.5, -0.5, -0.5));
	this->bb.setScale(this->scale);
	this->bb.setTranslation(this->center);

	this->affectByGravity = g;
	this->clippingEnabled = false;
}

Joint* Object::getStart() { return this->start; }
Joint* Object::getEnd() { return this->end; }
float Object::getMass() { return this->mass; }
Vector3 Object::getScale() { return this->scale; }
Vector3 Object::getRotation() { return this->rotation; }
Vector3 Object::getCenter() { return this->center; }
Vector3 Object::getMomentum() { return this->velocity * this->mass; };
BoundingBox& Object::getBoundingBox() { return this->bb; }
bool Object::isAffectByGravity() { return this->affectByGravity; }

bool Object::isClippingEnabled() { return this->clippingEnabled; }
void Object::setClipping(bool b) { this->clippingEnabled = b; }

Vector3 Object::getColour() { return this->colour; }
void Object::setColour(Vector3 v) { this->colour = v; }

void Object::constraint()
{
	if (this->start == nullptr) return;
	Vector3 a = this->start->getCurrent();
	Vector3 b = this->end->getCurrent();

	Vector3 diff = b - a;
	float ll = diff.Length();
	float fr = ((this->length - ll) / ll) / 2.0;

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
	this->velocity += a * dt;
	if (start == nullptr)
	{
		this->center += velocity * dt;
		this->bb.setTranslation(this->center);
	}
	else
	{
		if (!start->isFixed())
		{
			start->move(a * dt);
		}

		if (!end->isFixed())
		{
			end->move(a * dt);
		}

		this->constraint();
	}

}

void Object::applyImpulse(Vector3 impulse, float dt)
{
	if (mass == 0) return;
	Vector3 a = Vector3(impulse.x / mass, impulse.y / mass, impulse.z / mass);
	this->accelerate(a, dt);
}

void Object::applyCircularImpulse(Vector3 impulse, float dt)
{
	if (mass == 0) return;
	Vector3 a = Vector3(impulse.x / mass, impulse.y / mass, impulse.z / mass);
	if (!start->isFixed()) start->move(a * dt);
	if (!end->isFixed()) end->move(a * dt);
	this->constraint();
}