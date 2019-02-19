#pragma once
#include "block.h"
#include "Vector3.h"
#include <stdio.h>
#include "MyMath.h"

class BlockGenerator
{
public:
	static BlockGenerator* GetInstance();
	void generateBlocks(int block, int maxHeight, int maxLength, int offsetPos, int boundary);
	int randomDirection(int lastDir);
	bool checkWorldSpace(Vector3 pos);
	block *getHead();
	block *getTail();
private:
	enum DIRECTION
	{
		UP,
		LEFT,
		RIGHT,
		FORWARD,
		BACKWARD,
		NUM_DIRECTION
	};
	BlockGenerator();
	static BlockGenerator *instance;
	block *head, *tail;
	int lastDirection;
	~BlockGenerator();
};