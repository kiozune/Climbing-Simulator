#include "Player.h"

Player::Player()
{
	id = 0;
	energy = 5;
	alive = true;
}

unsigned Player::getId() { return this->id; }
void Player::setId(unsigned i) { this->id = i; }

void Player::setAlive(bool i) { this->alive = i; }

bool Player::isAlive()
{
	return alive;
}

void Player::recover(float dt) 
{ 
	this->energy += dt; 
	if (this->energy > 5)
		this->energy = 5;
}

void Player::tire(float dt) 
{ 
	this->energy -= dt * 2;
	if (this->energy < 0)
		this->energy = 0;
}

float Player::getEnergy() { return this->energy; }

Object* Player::getLeftHand() { return this->leftHand; }
void Player::setLeftHand(Object* obj) { this->leftHand = obj; }

Object * Player::getLeftArm() { return this->leftArm; }
void Player::setLeftArm(Object* obj) { this->leftArm = obj; }

Joint* Player::getLeftFingers() { return this->leftFingers; }

void Player::setLeftFingers(Joint* joint) 
{ 
	this->leftFingers = joint; 
	this->leftSpring = new Spring(this->leftFingers, this->leftFingers, 1, 1, 1);
	this->leftSpring->disable();
}



Spring** Player::getLeftSpring() { return &this->leftSpring; }
bool Player::isGrabbingLeft() { return this->leftSpring && this->leftSpring->isEnabled(); }

void Player::leftGrab(Joint* joint) 
{ 
	if (joint == nullptr)
	{
		Joint* fixed = new Joint(*(this->leftFingers));
		fixed->setFixed(true);
		this->leftSpring->changeEnd(fixed);
	}
	else
	{
		this->leftSpring->changeEnd(joint);
	}

	this->leftSpring->enable();
}

void Player::releaseLeft() { this->leftSpring->disable(); }



Object* Player::getRightHand() { return this->rightHand; }
void Player::setRightHand(Object* obj) { this->rightHand = obj; }

Object * Player::getRightArm() { return this->rightArm; }
void Player::setRightArm(Object* obj) { this->rightArm = obj; }

Joint* Player::getRightFingers() { return this->righFingers; }

void Player::setRightFingers(Joint* joint) 
{ 
	this->righFingers = joint; 
	this->rightSpring = new Spring(this->righFingers, this->righFingers, 1, 1, 1);
	this->rightSpring->disable();
}



Spring** Player::getRightSpring() { return &this->rightSpring; }
bool Player::isGrabbingRight() { return this->rightSpring && this->rightSpring->isEnabled(); }

void Player::rightGrab(Joint* joint) 
{ 
	if (joint == nullptr)
	{
		Joint* fixed = new Joint(*(this->righFingers));
		fixed->setFixed(true);
		this->rightSpring->changeEnd(fixed);
	}
	else
	{
		this->rightSpring->changeEnd(joint);
	}

	this->rightSpring->enable();
}

void Player::releaseRight() { this->rightSpring->disable(); }



Object* Player::getBody() { return this->body; }
void Player::setBody(Object* obj) { this->body = obj; }

std::vector<Object*> Player::getParts() { return this->parts; }
void Player::setParts(std::vector<Object*> v) {  this->parts = v; }