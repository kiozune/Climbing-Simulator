#include "Object.h"

Bone* Object::getBone() { return this->bone; }
void Object::setBone(Bone* bone) { this->bone = bone; }

void Object::applyImpulse(Vector3 force, float dt)
{
	this->bone->accelerate(force, dt);
}