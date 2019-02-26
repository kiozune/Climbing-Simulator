#include "RemotePlayer.h"

void RemotePlayer::update(PlayerData data)
{
	this->id = data.id;

	this->parts[0]->getStart()->setPosition(data.leftFingers);
	this->parts[0]->getEnd()->setPosition(data.leftWrist);

	this->parts[2]->getEnd()->setPosition(data.leftElbow);

	this->parts[3]->getStart()->setPosition(data.rightFingers);
	this->parts[3]->getEnd()->setPosition(data.rightWrist);

	this->parts[5]->getEnd()->setPosition(data.rightElbow);

	this->parts[6]->getStart()->setPosition(data.chest);
	this->parts[6]->getEnd()->setPosition(data.head);

	this->parts[7]->getEnd()->setPosition(data.pelvis);

	this->parts[8]->getEnd()->setPosition(data.leftFeet);
	this->parts[9]->getEnd()->setPosition(data.rightFeet);

}	