#pragma once
#include "block.h"
#include "Vector3.h"
#include <stdio.h>
#include <fstream>
#include "MyMath.h"

class BlockGenerator
{
public:
	static BlockGenerator* GetInstance();
	void generateBlocks();
	void generateBlocks(int block, int maxHeight, int maxLength, int offsetPos, int boundary);
	block *getHead();
	block *getTail();
private:
	enum DIRECTION
	{
		LEFT,
		RIGHT,
		FORWARD,
		BACKWARD,
		NUM_DIRECTION
	};
	
	BlockGenerator();
	static BlockGenerator *instance;

	int randomDirection(int lastDir);
	bool checkWorldSpace(Vector3 pos);
	void getLevelData();

	int *level;
	int sizeX, sizeY;

	block *head, *tail;
	int oppDirection;
	bool goUP;
	~BlockGenerator();
};