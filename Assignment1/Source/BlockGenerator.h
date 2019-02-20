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
	void generateBlocks(int offsetPos);
	block *getHead();
	block *getTail();
private:
	enum DIRECTION
	{
		LEFT,
		RIGHT,
		FORWARD,
		BACKWARD,
		UPWARD,
		NUM_DIRECTION
	};
	enum LEVEL
	{
		STANDARD,
		INCLINE,
		ROUNDABOUT,
		NUM_LEVEL
	};
	BlockGenerator();
	static BlockGenerator *instance;

	void getLevelData(std::string val, Vector3 pos, int offset);

	block *head, *tail;
	int oppDirection;
	bool goUP;
	~BlockGenerator();
};