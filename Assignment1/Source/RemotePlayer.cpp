#include "RemotePlayer.h"

void RemotePlayer::update(PlayerData data)
{
	this->parts[0]->getStart()->setCurrent(data.leftFingers);
	this->parts[0]->getEnd()->setCurrent(data.leftWrist);

	this->parts[2]->getEnd()->setCurrent(data.leftElbow);

	this->parts[3]->getStart()->setCurrent(data.rightFingers);
	this->parts[3]->getEnd()->setCurrent(data.rightWrist);

	this->parts[5]->getEnd()->setCurrent(data.rightElbow);

	this->parts[6]->getStart()->setCurrent(data.chest);
	this->parts[6]->getEnd()->setCurrent(data.head);

	this->parts[7]->getEnd()->setCurrent(data.pelvis);

	this->parts[8]->getEnd()->setCurrent(data.leftFeet);
	this->parts[9]->getEnd()->setCurrent(data.rightFeet);
}	