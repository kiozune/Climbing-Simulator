#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include "Vector3.h"

/*
	Strucutre

	leftHand, leftArm, leftBicep,
	rightHand, rightArm, rightBicep,
	neck, body,
	leftLeg, rightLeg

*/

struct PlayerData 
{
	Vector3 leftFingers, leftWrist, leftElbow,
			rightFingers, rightWrist, rightElbow,
			head, chest, pelvis,
			leftFeet, rightFeet;
};

#endif