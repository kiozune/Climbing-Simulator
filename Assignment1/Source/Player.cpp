#include "Player.h"

Object * Player::getLeftArm() { return this->leftArm; }
void Player::setLeftArm(Object* obj) { this->leftArm = obj; }

Joint * Player::getLeftHand() { return this->leftHand; }
void Player::setLeftHand(Joint* joint) { this->leftHand = joint; }
void Player::grabLeft() { this->leftHand->setFixed(true); }
void Player::releaseLeft() { this->leftHand->setFixed(false); }

Object * Player::getRightArm() { return this->rightArm; }
void Player::setRightArm(Object* obj) { this->rightArm = obj; }

Joint * Player::getRightHand() { return this->rightHand; }
void Player::setRightHand(Joint* joint) { this->rightHand = joint; }
void Player::grabRight() { this->rightHand->setFixed(true); }
void Player::releaseRight() { this->rightHand->setFixed(false); }

Object* Player::getBody() { return this->body; }
void Player::setBody(Object* obj) { this->body = obj; }