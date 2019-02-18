#include "BlockGenerator.h"
/*
Rule for PCG sp2 walker algorithm (Distribution method)
Rule 1 - Doesn't go back the same way it came from and the same direction it is heading to
Rule 2 - Choose a random length from maximum allowed length and draw it at given direction
Rule 3 - Avoids the edges of the map
Rule 4 - Spawn block till it runs out
*/

BlockGenerator* BlockGenerator::instance = 0;

BlockGenerator::BlockGenerator()
{
	head = nullptr;
	tail = nullptr;
	lastDirection = 0;
}

BlockGenerator * BlockGenerator::GetInstance()
{
	if (instance == nullptr)
		instance = new BlockGenerator();
	return instance;
}

void BlockGenerator::generateBlocks(int valBlocks, int maxHeight, int maxLength, int offsetPos, int boundary)
{
	Vector3 currentPos = { 0,0,0 };
	int blockTemp = valBlocks; // blockTemp val will need to decrease during whileloop
	
	// first block
	{
		block *temp = new block;
		temp->setVector3(currentPos);
		temp->setMesh();
		temp->setPrevious(nullptr); // starting block doesn't have a pervious block
		head = temp;
		tail = temp;
	}
	
	while (blockTemp) // Rule 4 | loop till there is no more blocks to build
	{
		switch (randomDirection(lastDirection))
		{
		case UP:
		{
			int randomHeight = rand() % maxHeight + 1;
			while (randomHeight) // rule 2
			{ // rule 3 doesn't applied to height
				if (blockTemp == 0)
					break;
				block* temp = new block;
				currentPos.y += offsetPos;
				temp->setVector3(currentPos);
				temp->setMesh();
				tail->setNext(temp);
				temp->setPrevious(tail);
				tail = temp;
				--blockTemp;
				--randomHeight;
			}
			lastDirection = UP;
		}
		break;
		case LEFT:
		{
			int randomLength = rand() % maxLength + 1; 
			while (randomLength) // rule 2
			{
				if (currentPos.x == boundary) // rule 3
					break;
				else if (blockTemp == 0)
					break;
				block* temp = new block;
				currentPos.x -= offsetPos;
				temp->setVector3(currentPos);
				temp->setMesh();
				tail->setNext(temp);
				temp->setPrevious(tail);
				tail = temp;
				--blockTemp;
				--randomLength;
			}
			lastDirection = LEFT;
		}
		break;
		case RIGHT:
		{
			int randomLength = rand() % maxLength + 1;
			while (randomLength) // rule 2
			{
				if (currentPos.x == boundary) // rule 3
					break;
				else if (blockTemp == 0)
					break;
				block* temp = new block;
				currentPos.x += offsetPos;
				temp->setVector3(currentPos);
				temp->setMesh();
				tail->setNext(temp);
				temp->setPrevious(tail);
				tail = temp;
				--blockTemp;
				--randomLength;
			}
			lastDirection = RIGHT;
		}
		break;
		case FORWARD:
		{
			int randomLength = rand() % maxLength + 1;
			while (randomLength) // rule 2
			{
				if (currentPos.z == boundary) // rule 3
					break;
				else if (blockTemp == 0)
					break;
				block * temp = new block;
				currentPos.z += offsetPos;
				temp->setVector3(currentPos);
				temp->setMesh();
				tail->setNext(temp);
				temp->setPrevious(tail);
				tail = temp;
				--blockTemp;
				--randomLength;
			}
			lastDirection = FORWARD;
		}
		break;
		case BACKWARD:
		{
			int randomLength = rand() % maxLength + 1;
			while (randomLength) // rule 2
			{
				if (currentPos.z == boundary) // rule 3
					break;
				else if (blockTemp == 0)
					break;
				block * temp = new block;
				currentPos.z -= offsetPos;
				temp->setVector3(currentPos);
				temp->setMesh();
				tail->setNext(temp);
				temp->setPrevious(tail);
				tail = temp;
				--blockTemp;
				--randomLength;
			}
			lastDirection = BACKWARD;
		}
		break;
		default:
			printf("ERROR at switch case > blockGenerator.cpp > generateblocks function");
			break;
		}
	}
}

int BlockGenerator::randomDirection(int lastDir)
{
	int temp = 0;
	int oppDir = 0;

	switch (lastDir) // flip the direction
	{
	case UP:
		oppDir = 0; // blank
		break;
	case LEFT:
		oppDir = RIGHT;
		break;
	case RIGHT:
		oppDir = LEFT;
		break;
	case FORWARD:
		oppDir = BACKWARD;
		break;
	case BACKWARD:
		oppDir = FORWARD;
		break;
	default:
		printf("ERROR at switch case > blockGenerator.cpp > randomDirection function");
		break;
	}

	do
	{
		temp = rand() % NUM_DIRECTION; // 0 to 4
	} while (temp == lastDir || temp == oppDir); // Rule 1

	return temp;
}

block * BlockGenerator::getHead()
{
	return head;
}

block * BlockGenerator::getTail()
{
	return tail;
}

BlockGenerator::~BlockGenerator()
{
	block* current = head;

	while (current != tail)
	{
		block* temp = current->getNext();
		delete current;
		current = temp;
	}
	delete current;
}