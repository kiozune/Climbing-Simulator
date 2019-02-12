#ifndef LIGHT_H
#define LIGHT_H

#include "vertex.h"

class Light 
{

	std::string getGLName(std::string end);

public:
	static unsigned count;

	enum LIGHT_TYPE
	{
		POINT = 0,
		DIRECTIONAL,
		SPOT,
	};

	enum LIGHT_VAR {
		L_TYPE,
		L_POSITION,
		L_COLOR,
		L_POWER,
		L_KC,
		L_KL,
		L_KQ,
		L_COSCUTOFF,
		L_COSINNER,
		L_EXPONENT,
		L_SPOTDIRECTION,

		L_TOTAL
	};

	unsigned parameters[L_TOTAL];

	LIGHT_TYPE type;
	Vector3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;

	Position position;
	Color color;
	float power;
	float kC, kL, kQ; // C = constant, L = linear, Q = quadratic

	Light();

	void getUniformLocation(const unsigned id);
	void setUniform();

};

#endif