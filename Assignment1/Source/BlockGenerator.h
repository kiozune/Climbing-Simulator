#pragma once
#include "block.h"
#include "Vector3.h"
#include <stdio.h>

class BlockGenerator
{
public:
	static BlockGenerator* GetInstance();
	void generateBlocks(Vector3 startCoord, int maxHeight, int offsetPos, int boundary);
	block *getHead();
	block *getTail();
private:
	BlockGenerator();
	static BlockGenerator *instance;
	block *head, *tail;
	Vector3 currentPos;
	~BlockGenerator();
};