#pragma once
#include "Level.h"
#include "Vector3.h"
#include <stdio.h>

class LevelGenerator
{
public:
	LevelGenerator();
	void generateLevel(Vector3 minCoord,Vector3 maxCoord, int posOffset);
	
	~LevelGenerator();
private:
	level *head, *tail;
	int currentX, currentY;
};