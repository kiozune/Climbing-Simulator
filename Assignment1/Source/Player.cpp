#include "Player.h"

Object* Player::getLeftHand() { return this->leftHand; }
void Player::setLeftHand(Object* obj) { this->leftHand = obj; }

Object * Player::getLeftArm() { return this->leftArm; }
void Player::setLeftArm(Object* obj) { this->leftArm = obj; }

bool Player::isLeftGrabbing() { return this->leftFingers->isFixed(); }
void Player::setLeftFingers(Joint* joint) { this->leftFingers = joint; }
void Player::grabLeft() { this->leftFingers->setFixed(true); }
void Player::releaseLeft() { this->leftFingers->setFixed(false); }

Object* Player::getRightHand() { return this->rightHand; }
void Player::setRightHand(Object* obj) { this->rightHand = obj; }

Object * Player::getRightArm() { return this->rightArm; }
void Player::setRightArm(Object* obj) { this->rightArm = obj; }

bool Player::isRightGrabbing() { return this->righFingers->isFixed(); }
void Player::setRightFingers(Joint* joint) { this->righFingers = joint; }
void Player::grabRight() { this->righFingers->setFixed(true); }
void Player::releaseRight() { this->righFingers->setFixed(false); }

Object* Player::getBody() { return this->body; }
void Player::setBody(Object* obj) { this->body = obj; }